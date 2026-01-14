#ifndef RENDER_TYPES_H
#define RENDER_TYPES_H
#include "core/utils.h"

#include <stddef.h>

struct Camera {
        f32 proj[16];
        f32 view[16];
        f32 position[3];
        f32 rotation[3];
        f32 fov;
        f32 near_plane;
        f32 far_plane;
};

struct GlobalRes {
        Array VAOs;
        Array shader_programs;
        Array mesh_objs;
};

struct MeshObj {
        f32 transform[16];
        u32 VAO;
        u32 VBO;
        u32 EBO;
        u32 shader_program;
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
};

#endif
