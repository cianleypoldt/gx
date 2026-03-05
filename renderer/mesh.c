#include "glad/glad.h"
#include "gx.h"

#include <stdio.h>

gx_mesh gx_mesh_create(gx_ctx *ctx, gx_mesh_desc mesh_desc)
{
	struct MeshObj mesh = { 0 };

	mesh.index_count = mesh_desc.index_count;

	assign_identity(mesh.ubo_data.transform);

	struct LayoutTemplate *layout =
		array_at(&ctx->glob_resources.layouts, mesh_desc.layout);

	glCreateBuffers(1, &mesh.VBO);
	glNamedBufferStorage(mesh.VBO, mesh_desc.vertex_data_size,
			     mesh_desc.vertex_data, 0);

	glCreateBuffers(1, &mesh.EBO);
	glNamedBufferStorage(mesh.EBO, mesh_desc.index_data_size,
			     mesh_desc.index_data, 0);

	glCreateVertexArrays(1, &mesh.VAO);

	// bind vertex buffer to binding index 0
	glVertexArrayVertexBuffer(mesh.VAO, 0, mesh.VBO, 0, layout->stride);

	// bind index buffer
	glVertexArrayElementBuffer(mesh.VAO, mesh.EBO);

	// apply layout template
	for (GLuint i = 0; i < layout->attrib_count; ++i) {
		const gx_layout_attrib *attr = &layout->attributes[i];

		GLenum gl_type;
		switch (attr->type) {
		case GX_TYPE_FLOAT:
			gl_type = GL_FLOAT;
			break;
		case GX_TYPE_INT:
			gl_type = GL_INT;
			break;
		case GX_TYPE_UNSIGNED_INT:
			gl_type = GL_UNSIGNED_INT;
			break;
		default:
			gl_type = GL_FLOAT;
			break;
		}

		glEnableVertexArrayAttrib(mesh.VAO, i);
		if (gl_type == GL_FLOAT) {
			glVertexArrayAttribFormat(mesh.VAO,
						  i, /* attribute location */
						  attr->count, gl_type,
						  attr->normalized,
						  attr->offset);
		} else {
			glVertexArrayAttribIFormat(mesh.VAO,
						   i, /* attribute location */
						   attr->count, gl_type,
						   attr->offset);
		}

		glVertexArrayAttribBinding(mesh.VAO, i, /* attribute location */
					   0 /* binding index */
		);
	}

	mesh.gx_id = ++ctx->gx_id_tracker;
	array_append(&ctx->glob_resources.mesh_objs, &mesh);
	return mesh.gx_id;
}

void gx_mesh_render(gx_ctx *ctx, gx_mesh mesh, gx_shader shader)
{
	struct MeshObj *m = NULL;
	// compute_transform(m->ubo_data.transform, m->position, m->quat_rotation);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	u32 cam_id = ctx->camera.gl_camera_object_id;

	struct GLCameraObject *cam_obj = NULL;
	for (int i = 0; i < ctx->glob_resources.gl_camera_objs.count; i++) {
		cam_obj = array_at(&ctx->glob_resources.gl_camera_objs, i);
		if (cam_obj->gx_id == cam_id) {
			// for (int row = 0; row < 4; row++) {
			// for (int col = 0; col < 4; col++) {
			// printf(" ; %f", cam_obj->ubo_data.view[4 * col + row]);
			// }
			// printf("\n");
			// }
			// printf("\n");
			// break;
		}
	}
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		printf("GL error: 0x%x\n", err);
	}
	glUseProgram(*(unsigned int *)array_at(
		&ctx->glob_resources.shader_programs, shader));
	glBindVertexArray(m->VAO);

	glDrawElements(GL_TRIANGLES, m->index_count, GL_UNSIGNED_INT,
		       (const void *)0);
}
