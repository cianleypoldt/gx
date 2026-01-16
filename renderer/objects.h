#ifndef RENDER_OBJECTS_H
#define RENDER_OBJECTS_H
#include "core/utils.h"
#include "gx.h"

#include <stddef.h>

struct GLCameraObject {
        u32 gx_id;
        u32 UBO;

        struct {
                f32 proj[16];
                f32 view[16];
        } ubo_data;
};

struct Camera {
        f32 position[3];
        f32 quat_rotation[4];

        f32 fov;
        f32 aspect;
        f32 near_plane;
        f32 far_plane;

        u32 gl_camera_object_id;
};

struct GlobalRes {
        Array layouts;
        Array shader_programs;
        Array gl_camera_objs;
        Array mesh_objs;
};

struct LayoutTemplate {
        gx_layout_attrib* attributes;
        int               attrib_count;
        size_t            stride;
};

struct MeshObj {
        u32 gx_id;

        f32 transform[16];
        u32 VAO;
        u32 VBO;
        u32 EBO;
        u32 vertex_count;
        u32 index_count;
};

struct gx_ctx {
        void*  window_ptr;
        size_t frame_width;
        size_t frame_height;
        int    is_fullscreen;
        f32    clear_color[4];

        struct Camera    camera;
        struct GlobalRes glob_resources;

        u32 gx_id_tracker;
};

#endif
