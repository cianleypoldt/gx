#include "common/dynamic_array.h"
#include "common/slot_map.h"
#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"

#define UNIFORM_NAME_BUFFER_SIZE 64

typedef struct {
	GLuint id;
	dynamic_array_t *uniforms;
} shader_program_t;

struct ShaderManager {
	slot_map_t *shader_programs;
};

typedef struct {
	char name[UNIFORM_NAME_BUFFER_SIZE];
	GLuint type;
	GLint location;
} uniform_t;

shadermgr_t *shadermgr_create()
{
	shadermgr_t *mgr = malloc(sizeof(shadermgr_t));
	mgr->shader_programs = sm_create(sizeof(shader_program_t));

	return mgr;
}

void shadermgr_destroy(shadermgr_t *mgr)
{
	for (index_t i = 0; i < sm_dense_length(mgr->shader_programs); i++) {
		shader_program_t *program =
			sm_at_index(mgr->shader_programs, i);
		glDeleteProgram(program->id);
	}

	sm_delete(mgr->shader_programs);
	free(mgr);
}

static int compile_with_logs(GLuint shader_id);
static int link_shader_with_logs(GLuint prog, GLuint vs, GLuint fs);

shader_id shadermgr_create_program(shadermgr_t *mgr, char *vs_src,
				   size_t vs_src_size, char *fs_src,
				   size_t fs_src_size)
{
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	{
		const char *vs_src_[] = { vs_src };
		const char *fs_src_[] = { fs_src };
		GLint vs_size_ = vs_src_size;
		GLint fs_size_ = fs_src_size;

		glShaderSource(vs, 1, vs_src_, &vs_size_);
		glShaderSource(fs, 1, fs_src_, &fs_size_);
	}

	if (!compile_with_logs(vs) || !compile_with_logs(fs)) {
		glDeleteShader(vs);
		glDeleteShader(fs);
		abort();
	}

	GLuint prog = glCreateProgram();
	if (!link_shader_with_logs(prog, vs, fs)) {
		glDeleteProgram(prog);
		glDeleteShader(vs);
		glDeleteShader(fs);
		abort();
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	shader_program_t sp;
	sp.id = prog;
	sp.uniforms = da_create(sizeof(uniform_t));

	GLint count;
	glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &count);
	da_resize(sp.uniforms, count);

	GLint size;
	GLenum type;
	GLsizei length;

	for (GLint i = 0; i < count; i++) {
		uniform_t *u = (uniform_t *)da_at(sp.uniforms, (index_t)i);

		glGetActiveUniform(prog, (GLuint)i, UNIFORM_NAME_BUFFER_SIZE,
				   &length, &size, &type, u->name);
		u->type = type;
		u->location = glGetUniformLocation(prog, u->name);
	}

	return sm_add(mgr->shader_programs, &sp);
}

void shadermgr_destroy_program(shadermgr_t *mgr, shader_id id)
{
	shader_program_t *shader_program = sm_at_id(mgr->shader_programs, id);
	glDeleteProgram(shader_program->id);
	da_delete(shader_program->uniforms);
	sm_remove_id(mgr->shader_programs, id);
}

uniform_id_t shadermgr_get_uniform(shadermgr_t *mgr, shader_id id,
				   const char *name)
{
	shader_program_t *prog = sm_at_id(mgr->shader_programs, id);
	for (index_t i = 0; i < da_length(prog->uniforms); i++) {
		uniform_t *u = da_at(prog->uniforms, i);
		if (strcmp(u->name, name) == 0)
			return i;
	}
	return SM_INVALID_INDEX;
}

static int compile_with_logs(GLuint shader_id)
{
	glCompileShader(shader_id);

	int success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (success == GL_TRUE)
		return 1;

	int len;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
	char *log = malloc(len + 1);
	glGetShaderInfoLog(shader_id, len, NULL, log);
	log[len] = '\0';
	printf("Shader failed to compile: \n");
	printf("%s\n", log);
	free(log);
	return 0;
}

static int link_shader_with_logs(GLuint prog, GLuint vs, GLuint fs)
{
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);

	GLint linked = 0;
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	if (linked)
		return 1;

	GLint len = 0;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	char *log = malloc(len ? len : 1);
	glGetProgramInfoLog(prog, len, NULL, log);
	printf("Failed to link shaders: \n");
	printf("%s\n", log);
	free(log);

	return 0;
}
