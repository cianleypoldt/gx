#include "common/dynamic_array.h"
#include "glad/glad.h"
#include "mesh_buffer.h"
#include "common/slot_map.h"
#include "render/public.h"
#include <stdlib.h>
#include <string.h>

struct Layout {
	struct LayoutAttribute attributes[GX_MAX_LAYOUT_ATTRIBUTES];
	size_t attrib_count;
	size_t stride;
};

struct MeshBufferManager {
	dynamic_array_t *layouts;
	slot_map_t *mesh_buffers;
};

struct MeshBuffer {
	index_t index_count;
	GLuint VAO, VBO, EBO;
};

mbmgr_t *mbmgr_init()
{
	mbmgr_t *mgr = malloc(sizeof(mbmgr_t));
	mgr->mesh_buffers = sm_create(sizeof(struct MeshBuffer));
	mgr->layouts = da_create(sizeof(struct Layout));
	return mgr;
}
void mbmgr_deinit(mbmgr_t *mgr)
{
	for (index_t i = 0; i < sm_dense_length(mgr->mesh_buffers); i++) {
		struct MeshBuffer *mb = sm_at_index(mgr->mesh_buffers, i);
		glDeleteBuffers(1, &mb->VBO);
		glDeleteBuffers(1, &mb->EBO);
		glDeleteVertexArrays(1, &mb->VAO);
	}

	sm_delete(mgr->mesh_buffers);
	free(mgr);
}

layout_index mbmgr_add_layout(mbmgr_t *mgr, struct LayoutAttribute *attributes,
			      index_t attrib_count, size_t stride)
{
	if (attrib_count > GX_MAX_LAYOUT_ATTRIBUTES || attrib_count <= 0) {
		abort();
	}
	struct Layout layout = { .attrib_count = attrib_count,
				 .stride = stride };
	memcpy(layout.attributes, attributes,
	       sizeof(struct LayoutAttribute) * attrib_count);
	da_append(mgr->layouts, &layout);
	return da_length(mgr->layouts) - 1;
}

mesh_buffer_id mbmgr_create_buffer(mbmgr_t *mgr, layout_index vertex_layout,
				   void *vertex_data, size_t vertex_data_size,
				   void *index_data, index_t index_count)
{
	struct MeshBuffer mb = { 0 };
	mb.index_count = index_count;

	glGenVertexArrays(1, &mb.VAO);
	glGenBuffers(1, &mb.VBO);
	glGenBuffers(1, &mb.EBO);

	glBindVertexArray(mb.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, mb.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertex_data,
		     GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mb.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mb.index_count * sizeof(uint32_t),
		     index_data, GL_STATIC_DRAW);

	struct Layout *layout = da_at(mgr->layouts, vertex_layout);
	index_t pointer_offset = 0;
	for (index_t i = 0; i < layout->attrib_count; i++) {
		struct LayoutAttribute *attrib = &layout->attributes[i];
		GLuint normalized = attrib->normalized ? GL_TRUE : GL_FALSE;

		switch (attrib->type) {
		case GX_FLOAT_ATTRIB:
			glVertexAttribPointer(i, attrib->count, GL_FLOAT,
					      normalized, layout->stride,
					      (void *)pointer_offset);
			pointer_offset += attrib->count * sizeof(float);
			break;
		case GX_INT_ATTRIB:
			glVertexAttribIPointer(i, attrib->count, GL_INT,
					       layout->stride,
					       (void *)pointer_offset);
			pointer_offset += attrib->count * sizeof(int);
			break;
		case GX_UNSIGNED_INT_ATTRIB:
			glVertexAttribIPointer(i, attrib->count,
					       GL_UNSIGNED_INT, layout->stride,
					       (void *)pointer_offset);
			pointer_offset += attrib->count * sizeof(unsigned int);
			break;
		default:
			abort();
		}
		glEnableVertexAttribArray((GLuint)i);
	}

	return sm_add(mgr->mesh_buffers, &mb);
}

void mbmgr_destroy_buffer(mbmgr_t *mgr, mesh_buffer_id id)
{
	struct MeshBuffer *mb = sm_at_id(mgr->mesh_buffers, id);
	glDeleteBuffers(1, &mb->VBO);
	glDeleteBuffers(1, &mb->EBO);
	glDeleteVertexArrays(1, &mb->VAO);
}
