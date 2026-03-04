#include "common/def.h"
#include "glad/glad.h"
#include "mesh_buffer.h"
#include "common/slot_map.h"
#include <stdlib.h>

struct MeshBufferManager {
	slot_map_t *mesh_buffers;
};

struct MeshBuffer {
	index_t index_count;
	index_t vertex_count;
	GLuint VAO, VBO, EBO;
};

mbmgr_t *mbmgr_create()
{
	mbmgr_t *mgr = malloc(sizeof(mbmgr_t));
	mgr->mesh_buffers = sm_create(sizeof(struct MeshBuffer));
	return mgr;
}
void mbmgr_destroy(mbmgr_t *mgr)
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

mesh_buffer_id mbmgr_create_buffer(mbmgr_t *mgr, void *vertex_data,
				   size_t vertex_data_size, void *index_data,
				   size_t index_data_size)
{
	struct MeshBuffer mesh = { 0 };

	glCreateBuffers(1, &mesh.VBO);
	glNamedBufferStorage(mesh.VBO, vertex_data_size, vertex_data, 0);

	glCreateBuffers(1, &mesh.EBO);
	glNamedBufferStorage(mesh.EBO, index_data_size, index_data, 0);

	glCreateVertexArrays(1, &mesh.VAO);

	// bind vertex buffer to binding index 0
	glVertexArrayVertexBuffer(mesh.VAO, 0, mesh.VBO, 0,
				  10000); // big stride

	// bind index buffer
	glVertexArrayElementBuffer(mesh.VAO, mesh.EBO);
}

void mbmgr_destroy_buffer(mbmgr_t *mgr, mesh_buffer_id id);

void mbmgr_update_buffer(mbmgr_t *mgr, mesh_buffer_id id, const void *data);
