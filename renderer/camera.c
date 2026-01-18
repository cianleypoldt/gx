#include "core/utils.h"
#include "glad/glad.h"
#include "gx.h"
#include "renderer/objects.h"

#include <stdio.h>

void init_camera_ubo(gx_ctx* ctx) {
        struct GLCameraObject gl_camera_obj = { 0 };
        glGenBuffers(1, &gl_camera_obj.UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, gl_camera_obj.UBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(gl_camera_obj.ubo_data),
                     &gl_camera_obj.ubo_data, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, gl_camera_obj.UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        ctx->camera.gl_camera_object_id = gl_camera_obj.gx_id =
                ctx->gx_id_tracker++;
        array_append(&ctx->glob_resources.gl_camera_objs, &gl_camera_obj);
}

void sync_camera_ubo(gx_ctx* ctx) {
        for (size_t i = 0; i < ctx->glob_resources.gl_camera_objs.count; i++) {
                struct GLCameraObject* cam_obj =
                        array_at(&ctx->glob_resources.gl_camera_objs, i);

                if (ctx->camera.gl_camera_object_id == cam_obj->gx_id) {
                        glBindBuffer(GL_UNIFORM_BUFFER, cam_obj->UBO);
                        glBufferSubData(GL_UNIFORM_BUFFER, 0,
                                        sizeof(cam_obj->ubo_data),
                                        &cam_obj->ubo_data);
                        glBindBuffer(GL_UNIFORM_BUFFER, 0);
                        break;
                }
        }
}

void update_camera_projection(gx_ctx* ctx) {
        float* proj_mat4 = NULL;
        for (size_t i = 0; i < ctx->glob_resources.gl_camera_objs.count; i++) {
                struct GLCameraObject* cam_obj =
                        array_at(&ctx->glob_resources.gl_camera_objs, i);

                if (ctx->camera.gl_camera_object_id == cam_obj->gx_id) {
                        proj_mat4 = cam_obj->ubo_data.proj;
                }
        }
        if (proj_mat4) {
                compute_projection(proj_mat4, ctx->camera.fov,
                                   ctx->camera.aspect, ctx->camera.near_plane,
                                   ctx->camera.far_plane);
        }
        sync_camera_ubo(ctx);
}

void update_camera_view(gx_ctx* ctx) {
        float* view_mat4 = NULL;
        for (size_t i = 0; i < ctx->glob_resources.gl_camera_objs.count; i++) {
                struct GLCameraObject* cam_obj =
                        array_at(&ctx->glob_resources.gl_camera_objs, i);

                if (ctx->camera.gl_camera_object_id == cam_obj->gx_id) {
                        view_mat4 = cam_obj->ubo_data.view;
                }
        }
        if (view_mat4) {
                compute_view(view_mat4, ctx->camera.position,
                             ctx->camera.quat_rotation);
        }
        sync_camera_ubo(ctx);
}

void init_camera(gx_ctx* ctx) {
        init_camera_ubo(ctx);
        ctx->camera.fov        = 45.0f;
        ctx->camera.aspect     = 16.0f / 9.0f;
        ctx->camera.far_plane  = 1000.0f;
        ctx->camera.near_plane = 0.1f;

        ctx->camera.position[0] = 0;
        ctx->camera.position[1] = 0;
        ctx->camera.position[2] = 0;

        ctx->camera.quat_rotation[0] = 0;
        ctx->camera.quat_rotation[1] = 0;
        ctx->camera.quat_rotation[2] = 0;
        ctx->camera.quat_rotation[3] = 1;

        update_camera_projection(ctx);
        update_camera_view(ctx);
}

void gx_camera_set_position(gx_ctx* ctx, float* vec3_position) {
        ctx->camera.position[0] = vec3_position[0];
        ctx->camera.position[1] = vec3_position[1];
        ctx->camera.position[2] = vec3_position[2];
        update_camera_view(ctx);
}

void gx_camera_set_rotation(gx_ctx* ctx, float* quat_rotation) {
        ctx->camera.quat_rotation[0] = quat_rotation[0];
        ctx->camera.quat_rotation[1] = quat_rotation[1];
        ctx->camera.quat_rotation[2] = quat_rotation[2];
        ctx->camera.quat_rotation[3] = quat_rotation[3];
        update_camera_view(ctx);
}
