#include "common/slot_map.h"
#include "third_party/glad/glad.h"
#include <stdio.h>
#include <stdlib.h>
#include "UBO_mgr.h"

typedef struct {
	GLuint id;
	size_t size;
	GLuint binding;
} ubo_buffer_t;

struct UBO_Manager {
	slot_map_t *buffers;
	uint next_binding;
	GLint max_bindings;
};

ubomgr_t *ubomgr_create()
{
	ubomgr_t *mgr = malloc(sizeof(ubomgr_t));
	mgr->buffers = sm_create(sizeof(ubo_buffer_t));
	mgr->next_binding = 0;

	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &mgr->max_bindings);

	return mgr;
}

void ubomgr_destroy(ubomgr_t *mgr)
{
	for (index_t i = 0; i < sm_dense_length(mgr->buffers); i++) {
		ubo_buffer_t *ubo_buffer = sm_at_index(mgr->buffers, i);
		glDeleteBuffers(1, &ubo_buffer->id);
	}

	sm_delete(mgr->buffers);
	free(mgr);
}

ubo_id ubomgr_create_ubo(ubomgr_t *mgr, size_t size)
{
	if (mgr->next_binding >= (GLuint)mgr->max_bindings) {
		fprintf(stderr, "UBO binding points exhausted\n");
		abort();
	}

	ubo_buffer_t buffer;
	buffer.size = size;
	glGenBuffers(1, &buffer.id);
	if (buffer.id == 0) {
		fprintf(stderr, "Failed to generate UBO\n");
		abort();
	}
	buffer.binding = mgr->next_binding++;

	glBindBuffer(GL_UNIFORM_BUFFER, buffer.id);

	glBufferData(GL_UNIFORM_BUFFER, buffer.size, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, buffer.binding, buffer.id);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return sm_add(mgr->buffers, &buffer);
}

void ubomgr_update_ubo(ubomgr_t *mgr, ubo_id id, const void *data)
{
	ubo_buffer_t *ubo_buffer = sm_at_id(mgr->buffers, id);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_buffer->id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, ubo_buffer->size, data);
}

void ubomgr_update_ubo_range(ubomgr_t *mgr, ubo_id id, const void *data,
			     size_t offset, size_t size)
{
	ubo_buffer_t *ubo_buffer = sm_at_id(mgr->buffers, id);
	if (offset + size > ubo_buffer->size) {
		fprintf(stderr, "UBO range update out of bounds\n");
		abort();
	}
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_buffer->id);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
