#include "common/def.h"
#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UBO.h"

#define GX_MAX_UNIFORM_BUFFERS 114

typedef struct {
	GLuint id;
	size_t size;
	GLuint binding;
} ubo_buffer_t;

struct UBO_Manager {
	ubo_buffer_t ubo_buffers[GX_MAX_UNIFORM_BUFFERS];
	char occup[GX_MAX_UNIFORM_BUFFERS];
	GLint max_bindings;
	GLint max_block_size;
};

ubomgr_t *ubomgr_init()
{
	GL_MAX_UNIFORM_BUFFER_BINDINGS;
	ubomgr_t *mgr = malloc(sizeof(ubomgr_t));

	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &mgr->max_bindings);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &mgr->max_block_size);

	memset(mgr->occup, 0, sizeof(mgr->occup));

	return mgr;
}

void ubomgr_deinit(ubomgr_t *mgr)
{
	for (index_t i = 0; i < GX_MAX_UNIFORM_BUFFERS; i++) {
		if (mgr->occup[i])
			glDeleteBuffers(1, &mgr->ubo_buffers[i].id);
	}
	free(mgr);
}

ubo_binding ubomgr_add_ubo(ubomgr_t *mgr, size_t size, ubo_binding binding)
{
	if (binding > (GLuint)mgr->max_bindings ||
	    binding > GX_MAX_UNIFORM_BUFFERS) {
		fprintf(stderr, "UBO binding points exhausted\n");
		abort();
	}
	if (size > (size_t)mgr->max_block_size) {
		fprintf(stderr, "Max UBO size exceeded.\n");
		abort();
	}
	if (mgr->occup[binding]) {
		fprintf(stderr, "UBO binding point %i already in use.\n",
			(int)binding);
		abort();
	}

	ubo_buffer_t *buffer = &mgr->ubo_buffers[binding];
	glGenBuffers(1, &buffer->id);
	if (buffer->id == 0) {
		fprintf(stderr, "Failed to generate UBO\n");
		abort();
	}
	mgr->occup[binding] = 1;
	buffer->size = size;
	buffer->binding = binding;

	glBindBuffer(GL_UNIFORM_BUFFER, buffer->id);

	glBufferData(GL_UNIFORM_BUFFER, buffer->size, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, buffer->binding, buffer->id);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return binding;
}

void ubomgr_update_ubo(ubomgr_t *mgr, ubo_binding index, const void *data)
{
	if (!mgr->occup[index])
		abort();
	glBindBuffer(GL_UNIFORM_BUFFER, mgr->ubo_buffers[index].id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mgr->ubo_buffers[index].size,
			data);
}

void ubomgr_update_ubo_range(ubomgr_t *mgr, ubo_binding index, const void *data,
			     size_t offset, size_t size)
{
	if (!mgr->occup[index])
		abort();
	if (offset + size > mgr->ubo_buffers[index].size) {
		fprintf(stderr, "UBO range update out of bounds\n");
		abort();
	}
	glBindBuffer(GL_UNIFORM_BUFFER, mgr->ubo_buffers[index].id);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
