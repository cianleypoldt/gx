#include "render/material.h"
#include "common/dynamic_array.h"
#include "render/shader.h"
#include "ubiform_buffers.h"
#include "render/UBO.h"
#include <stdlib.h>

#define GX_MAX_UNIFORM_BUFFERS 114

struct MaterialManager {
	dynamic_array_t *materials;
	ubo_binding material_ubo;
};

struct Material {
	struct MaterialUBOData ubo_data;
	shader_id shader;
};

matmgr_t *matmgr_init(ubo_binding ubo)
{
	matmgr_t *mgr = malloc(sizeof(matmgr_t));
	mgr->materials = da_create(sizeof(struct Material));
	mgr->material_ubo = ubo;
	return mgr;
}

void matmgr_deinit(matmgr_t *mgr)
{
	da_delete(mgr->materials);
	free(mgr);
}

material_id matmgr_add_material(matmgr_t *mgr, struct MaterialDesc *mat_desc,
				shader_id shader)
{
	struct Material m = { .ubo_data.metallic = mat_desc->metallic,
			      .ubo_data.roughness = mat_desc->roughness };
	da_append(mgr->materials, mat_desc);
	return da_length(mgr->materials) - 1;
}

void matmgr_buffer_mat(matmgr_t *mgr, ubomgr_t *ubomgr, material_id id)
{
	ubomgr_update_ubo(
		ubomgr, mgr->material_ubo,
		&((*(struct Material *)da_at(mgr->materials, id)).ubo_data));
}
