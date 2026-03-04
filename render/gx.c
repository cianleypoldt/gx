#include "gx.h"
#include "UBO.h"
#include "common/utils.h"
#include "shader.h"
#include "camera.h"
#include "material.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>

struct GX_Context {
	void *window;
	ubomgr_t *ubomgr;
	shadermgr_t *shadermgr;

	ubo_id camera_ubo;
	ubo_id material_ubo;

	struct Camera camera;
};

gxctx *gx_init()
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
	// file vs = load_file("shaders/vs.glsl");
	// file fs = load_file("shaders/fs.glsl");
	//
	// shader_id dafault = shadermgr_create_program(
	// ctx->shadermgr, vs.buffer, vs.size, fs.buffer, fs.size);
	//
	// free_file(vs);
	// free_file(fs);
	//
	// shadermgr_log_uniforms(ctx->shadermgr, dafault);

	return ctx;
}

void gx_deinit(gxctx *ctx)
{
	shadermgr_deinit(ctx->shadermgr);
	ubomgr_deinit(ctx->ubomgr);

	window_destroy(ctx->window);
	glfw_deinit();
	free(ctx);
}
