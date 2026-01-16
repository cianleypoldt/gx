#include "core/utils.h"
#include "glad/glad.h"
#include "gx.h"
#include "renderer/objects.h"
#include "renderer/renderer.h"

#include <stdio.h>

gx_ctx* gx_ctx_init(int frame_width, int frame_height) {
        init_glfw();
        void* window_ptr =
                create_window(frame_width, frame_height, "gx_viewport");

        init_OpenGL(window_ptr);

        gx_ctx* ctx = malloc(sizeof(gx_ctx));
        memset(ctx, 0, sizeof(gx_ctx));
        ctx->window_ptr   = window_ptr;
        ctx->frame_height = frame_height;
        ctx->frame_width  = frame_width;

        ctx->glob_resources.shader_programs = array_create(sizeof(u32));
        ctx->glob_resources.layouts =
                array_create(sizeof(struct LayoutTemplate));
        ctx->glob_resources.mesh_objs = array_create(sizeof(struct MeshObj));
        ctx->glob_resources.gl_camera_objs =
                array_create(sizeof(struct GLCameraObject));
        ctx->is_fullscreen = 0;

        init_camera(ctx);

        return ctx;
}

void gx_ctx_drop(gx_ctx* ctx) {
        if (ctx == NULL) {
                return;
        }
        for (int i = 0; i < ctx->glob_resources.layouts.count; i++) {
                free(((struct LayoutTemplate*) array_at(
                              &ctx->glob_resources.layouts, i))
                             ->attributes);
        }
        for (int i = 0; i < ctx->glob_resources.shader_programs.count; i++) {
                glDeleteProgram(*(unsigned int*) array_at(
                        &ctx->glob_resources.shader_programs, i));
        }
        for (int i = 0; i < ctx->glob_resources.mesh_objs.count; i++) {
                struct MeshObj* mesh_obj = (struct MeshObj*) array_at(
                        &ctx->glob_resources.mesh_objs, i);
                gx_mesh_delete(ctx, mesh_obj->gx_id);
        }
        for (int i = 0; i < ctx->glob_resources.mesh_objs.count; i++) {
                glDeleteBuffers(
                        1, &(*(struct GLCameraObject*) array_at(
                                     &ctx->glob_resources.gl_camera_objs, i))
                                    .UBO);
        }

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
                printf("OpenGL error: %u\n", err);
        }

        destroy_window(ctx->window_ptr);
        terminate_glfw();
        array_delete(&ctx->glob_resources.layouts);
        array_delete(&ctx->glob_resources.shader_programs);
        array_delete(&ctx->glob_resources.gl_camera_objs);
        array_delete(&ctx->glob_resources.mesh_objs);
        free(ctx);
}
