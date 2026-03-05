#ifndef CAMERA_H
#define CAMERA_H

#include "transform.h"

struct camera_ubo_data {
	float mat4_proj[16];
	float mat4_view[16];
};

struct Camera {
	struct Transform transform;
	float fov, aspect, near_plane, far_plane;
	int proj_dirty, view_dirty;
	struct camera_ubo_data ubo_data;
};

#endif
