#include "camera.h"
#include "render/UBO.h"
#include "render/public.h"
#include "common/mat4.h"
#include <stddef.h>
#include <string.h>

void camera_update(struct Camera *camera, ubomgr_t *ubomgr)
{
	if (!camera->view_dirty && !camera->proj_dirty)
		return;

	if (camera->proj_dirty) {
		struct CameraParameters *p = &camera->params;
		compute_projection(camera->ubo_data.mat4_proj, p->fov,
				   p->aspect, p->near_plane, p->far_plane);
	}

	if (camera->view_dirty) {
		compute_view(camera->ubo_data.mat4_view, camera->transform.pos,
			     camera->transform.rot);
	}

	if (camera->proj_dirty && camera->view_dirty) {
		ubomgr_update_ubo(ubomgr, camera->ubo_id, &camera->ubo_data);
	} else if (camera->proj_dirty) {
		ubomgr_update_ubo_range(
			ubomgr, camera->ubo_id, &camera->ubo_data,
			offsetof(struct CameraUBOData, mat4_proj),
			sizeof(camera->ubo_data.mat4_proj));
	} else {
		ubomgr_update_ubo_range(
			ubomgr, camera->ubo_id, &camera->ubo_data,
			offsetof(struct CameraUBOData, mat4_view),
			sizeof(camera->ubo_data.mat4_view));
	}

	camera->view_dirty = 0;
	camera->proj_dirty = 0;
}

void camera_set_transform(struct Camera *camera, struct Transform *transform)
{
	if (memcmp(transform, &camera->transform, sizeof(struct Transform)) !=
	    0) {
		memcpy(&camera->transform, transform, sizeof(struct Transform));
		camera->view_dirty = 1;
	}
}

void camera_set_params(struct Camera *camera, struct CameraParameters *params)
{
	if (memcmp(params, &camera->params, sizeof(struct CameraParameters)) !=
	    0) {
		memcpy(&camera->params, params,
		       sizeof(struct CameraParameters));
		camera->proj_dirty = 1;
	}
}
