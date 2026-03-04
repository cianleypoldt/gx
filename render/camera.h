#ifndef CAMERA_H
#define CAMERA_H

struct camera_ubo_data {
	float mat4_view[16];
	float mat4_proj[16];
};

struct Camera {
	float fov, aspect, near_plane, far_plane;
	struct camera_ubo_data ubo_data;
};

void look_at();
void set_pos();

#endif
