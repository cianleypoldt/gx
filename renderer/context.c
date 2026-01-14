#include "core/utils.h"
#include "glad/glad.h"
#include "gx.h"
#include "objects.h"

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

        ctx->glob_resources.buffer_layouts  = array_create(sizeof(u32));
        ctx->glob_resources.shader_programs = array_create(sizeof(u32));
        ctx->glob_resources.mesh_objs = array_create(sizeof(struct MeshObj));
        ctx->is_fullscreen            = 0;

        return ctx;
}

void gx_ctx_drop(gx_ctx* ctx) {
        if (ctx == NULL) {
                return;
        }
        for (int i = 0; i < ctx->glob_resources.buffer_layouts.count; i++) {
        }
        for (int i = 0; i < ctx->glob_resources.shader_programs.count; i++) {
                glDeleteProgram(*(unsigned int*) array_at(
                        &ctx->glob_resources.shader_programs, i));
        }
        for (int i = 0; i < ctx->glob_resources.mesh_objs.count; i++) {
                struct MeshObj* mesh_obj = (struct MeshObj*) array_at(
                        &ctx->glob_resources.mesh_objs, i);

                glDeleteVertexArrays(1, &mesh_obj->VAO);
                glDeleteBuffers(1, &mesh_obj->EBO);
                glDeleteBuffers(1, &mesh_obj->VBO);
        }

        destroy_window(ctx->window_ptr);
        terminate_glfw();
        array_delete(&ctx->glob_resources.buffer_layouts);
        array_delete(&ctx->glob_resources.shader_programs);
        array_delete(&ctx->glob_resources.mesh_objs);
        free(ctx);
}
