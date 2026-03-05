#include "common/def.h"
#include "common/file.h"
#include "render/mesh_buffer.h"
#include <stdio.h>
#include "gx.h"

float triangle_vertices[] = { // position          // color
	0.0f, 0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -0.5f, -0.5f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  1.0f
};

index_t triangle_indices[] = { 0, 1, 2 };

int main()
{
	gxctx *ctx = gx_init();
	file vs = load_file("shaders/vs.glsl");
	file fs = load_file("shaders/fs.glsl");

	gx_shader default_program =
		gx_add_shader(ctx, vs.buffer, vs.size, fs.buffer, fs.size);

	free_file(vs);
	free_file(fs);

	// gx_shader_print_uniforms(ctx, default_program);

	gx_layout_attribute attribs[3] = {
		{ .type = GX_FLOAT_ATTRIB, .count = 3, .normalized = 0 },
		{ .type = GX_FLOAT_ATTRIB, .count = 3, .normalized = 0 }
	};

	layout_index pos_col_layout =
		gx_add_layout(ctx, attribs, 2, 6 * sizeof(float));

	float triangle_vertices[] = { // position          // color
				      0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
				      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
				      0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	index_t triangle_indices[] = { 0, 1, 2 };

	mesh_buffer_id mb = gx_add_mesh(ctx, pos_col_layout, triangle_vertices,
					sizeof(triangle_vertices),
					triangle_indices, 3);

	gx_remove_mesh(ctx, mb);

	gx_deinit(ctx);
	printf("loool\n");
}
