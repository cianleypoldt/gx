#ifndef CAMERA_H
#define CAMERA_H

#include "ubiform_buffers.h"
#include "public.h"
#include "render/UBO.h"

struct Camera {
	struct Transform transform;
	struct CameraParameters params;
	int proj_dirty, view_dirty;
	ubo_binding ubo_id;
	struct CameraUBOData ubo_data;
};

void camera_set_transform(struct Camera *camera, struct Transform *transform);
void camera_set_params(struct Camera *camera, struct CameraParameters *params);
void camera_update(struct Camera *camera, ubomgr_t *ubomgr);

#endif
