#include "gx.h"
#include "UBO.h"
#include "common/utils.h"
#include "render/mesh_buffer.h"
#include "render/transform.h"
#include "shader.h"
#include "camera.h"
#include "material.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct GX_Context {
	void *window;
	ubomgr_t *ubomgr;
	shadermgr_t *shadermgr;
	mbmgr_t *mbmgr;

	ubo_binding camera_ubo;
	ubo_binding material_ubo;

	struct Camera camera;
};

gxctx *gx_init(void)
{
	gxctx *ctx = malloc(sizeof(gxctx));

	glfw_init();
	ctx->window = window_create(800, 600, "gx_view");
	glad_init(ctx->window);

	ctx->ubomgr = ubomgr_init();
	ctx->camera_ubo =
		ubomgr_add_ubo(ctx->ubomgr, sizeof(struct camera_ubo_data));
	ctx->material_ubo =
		ubomgr_add_ubo(ctx->ubomgr, sizeof(struct material_ubo_data));

	ctx->shadermgr = shadermgr_init();
	ctx->mbmgr = mbmgr_init();

	memset(&ctx->camera, 0, sizeof(struct Camera));

	ctx->camera.proj_dirty = 1;
	ctx->camera.view_dirty = 1;

	return ctx;
}

void gx_deinit(gxctx *ctx)
{
	shadermgr_deinit(ctx->shadermgr);
	ubomgr_deinit(ctx->ubomgr);
	mbmgr_deinit(ctx->mbmgr);

	window_destroy(ctx->window);
	glfw_deinit();
	free(ctx);
}

gx_layout gx_add_layout(gxctx *ctx, gx_layout_attribute *attribs,
			index_t attrib_count, size_t stride)
{
	return (gx_layout)mbmgr_add_layout(ctx->mbmgr, attribs, attrib_count,
					   stride);
}

gx_mesh gx_add_mesh(gxctx *ctx, gx_layout vertex_layout, void *vertex_data,
		    size_t vertex_data_size, void *index_data,
		    index_t index_count)
{
	return (gx_mesh)mbmgr_create_buffer(ctx->mbmgr, vertex_layout,
					    vertex_data, vertex_data_size,
					    index_data, index_count);
}

void gx_remove_mesh(gxctx *ctx, gx_mesh mesh)
{
	mbmgr_destroy_buffer(ctx->mbmgr, mesh);
}

gx_shader gx_add_shader(gxctx *ctx, char *vs_src, size_t vs_src_size,
			char *fs_src, size_t fs_src_size)
{
	return (gx_shader)shadermgr_add_program(
		ctx->shadermgr, vs_src, vs_src_size, fs_src, fs_src_size);
}

void gx_remove_shader(gxctx *ctx, gx_shader shader)
{
	shadermgr_remove_program(ctx->shadermgr, shader);
}

void gx_shader_print_uniforms(gxctx *ctx, gx_shader shader)
{
	shadermgr_log_uniforms(ctx->shadermgr, shader);
}

void gx_set_camera_transform(gxctx *ctx, gx_transform_t *transform)
{
	if (memcmp(transform, &ctx->camera.transform,
		   sizeof(struct Transform)) != 0) {
		memcpy(&ctx->camera.transform, transform,
		       sizeof(struct Transform));
		ctx->camera.view_dirty = 1;
	}
}
