#include "common/def.h"
#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>
#include "UBO.h"

#define GX_MAX_UNIFORM_BUFFERS 114

typedef struct {
	GLuint id;
	size_t size;
	GLuint binding;
} ubo_buffer_t;

struct UBO_Manager {
	ubo_buffer_t ubo_buffers[GX_MAX_UNIFORM_BUFFERS];
	index_t buffer_count;
	GLint max_bindings;
	GLint max_block_size;
};

ubomgr_t *ubomgr_init()
{
	GL_MAX_UNIFORM_BUFFER_BINDINGS;
	ubomgr_t *mgr = malloc(sizeof(ubomgr_t));
	mgr->buffer_count = 0;

	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &mgr->max_bindings);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &mgr->max_block_size);

	return mgr;
}

void ubomgr_deinit(ubomgr_t *mgr)
{
	for (index_t i = 0; i < mgr->buffer_count; i++) {
		glDeleteBuffers(1, &mgr->ubo_buffers[i].id);
	}
	free(mgr);
}

ubo_binding ubomgr_add_ubo(ubomgr_t *mgr, size_t size)
{
	if (mgr->buffer_count >= (GLuint)mgr->max_bindings ||
	    mgr->buffer_count >= GX_MAX_UNIFORM_BUFFERS) {
		fprintf(stderr, "UBO binding points exhausted\n");
		abort();
	}
	if (size > (size_t)mgr->max_block_size) {
		fprintf(stderr, "Max UBO size exceeded.\n");
		abort();
	}
	index_t binding = mgr->buffer_count;

	ubo_buffer_t *buffer = &mgr->ubo_buffers[binding];
	glGenBuffers(1, &buffer->id);
	if (buffer->id == 0) {
		fprintf(stderr, "Failed to generate UBO\n");
		abort();
	}
	mgr->buffer_count++;
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
	glBindBuffer(GL_UNIFORM_BUFFER, mgr->ubo_buffers[index].id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mgr->ubo_buffers[index].size,
			data);
}

void ubomgr_update_ubo_range(ubomgr_t *mgr, ubo_binding index, const void *data,
			     size_t offset, size_t size)
{
	if (offset + size > mgr->ubo_buffers[index].size) {
		fprintf(stderr, "UBO range update out of bounds\n");
		abort();
	}
	glBindBuffer(GL_UNIFORM_BUFFER, mgr->ubo_buffers[index].id);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
