#ifndef RENDER_OBJECTS_H
#define RENDER_OBJECTS_H
#include "common/slot_map.h"
#include "common/utils.h"
#include "gx.h"

#include <stddef.h>

struct GLCameraObject {
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

	struct GLCameraObject gl_camera_object;
};

struct GlobalRes {
	slotmap_t *layouts;
	slotmap_t *shader_programs;
	slotmap_t *mesh_objs;
};

struct LayoutTemplate {
	gx_layout_attrib *attributes;
	u32 attrib_count;
	size_t stride;
};

struct mesh_ubo_data {
	f32 transform[16];
};

struct MeshObj {
	u32 gx_id;

	struct mesh_ubo_data ubo_data;

	f32 position[3];
	f32 quat_rotation[4];

	u32 VAO;
	u32 VBO;
	u32 EBO;
	u32 vertex_count;
	u32 index_count;
};

struct gx_ctx {
	void *window_ptr;
	size_t frame_width;
	size_t frame_height;
	int is_fullscreen;
	f32 clear_color[4];

	struct Camera camera;
	struct GlobalRes glob_resources;

	u32 mesh_data_ubo;

	u32 gx_id_tracker;
};

#endif
