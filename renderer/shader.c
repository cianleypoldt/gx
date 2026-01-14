#include "core/utils.h"
#include "glad/glad.h"
#include "gx.h"
#include "renderer/objects.h"

#include <stdio.h>

uint32_t compile_shader_from_path(const char* path, GLuint type) {
        file src_file = load_file(path);
        if (src_file.size < 1) {
                printf("Error loading shader from file \"");
                printf("%s", path);
                printf("\"\n");
                free_file(src_file);
                return 0;
        }

        GLuint shader = glCreateShader(type);

        const char* srcv[] = { src_file.buffer };
        GLint length[]     = { (GLint) src_file.size };  // no null termination
        glShaderSource(shader, 1, srcv, length);
        free_file(src_file);

        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_TRUE) {
                return shader;
        }

        int len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char* log = malloc(len + 1);
        glGetShaderInfoLog(shader, len, NULL, log);
        log[len] = '\n';
        printf("Shader loaded from \"");
        printf("%s", path);
        printf("\" failed to compile:\n    ");
        printf("%s\n", log);
        free(log);
        return 0;
}

shader_program gx_create_shader(gx_ctx*     ctx,
                                const char* vertex_shader_path,
                                const char* fragment_shader_path) {
        GLuint vs =
                compile_shader_from_path(vertex_shader_path, GL_VERTEX_SHADER);
        GLuint fs      = compile_shader_from_path(fragment_shader_path,

                                                  GL_FRAGMENT_SHADER);
        GLuint prog_id = glCreateProgram();
        glAttachShader(prog_id, vs);
        glAttachShader(prog_id, fs);
        glLinkProgram(prog_id);

        GLint linked = 0;
        glGetProgramiv(prog_id, GL_LINK_STATUS, &linked);
        if (!linked) {
                GLint len = 0;
                glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &len);
                char* log = malloc(len ? len : 1);
                glGetProgramInfoLog(prog_id, len, NULL, log);
                printf("Failed to link \"%s", vertex_shader_path);
                printf("\" with \"%s", fragment_shader_path);
                printf("\" --- %s\n", log);
                free(log);
                glDeleteProgram(prog_id);
                glDeleteShader(vs);
                glDeleteShader(fs);
                shader_program null_program = { 0 };
                return null_program;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
        array_append(&ctx->glob_resources.shader_programs, &prog_id);

        shader_program program;
        program.id = prog_id;
        return program;
}
