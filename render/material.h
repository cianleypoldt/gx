#ifndef MATERIAL_H
#define MATERIAL_H

#include "common/def.h"
#include "render/UBO.h"
#include "render/public.h"
#include "render/shader.h"

typedef index_t material_id;
typedef struct MaterialManager matmgr_t;

matmgr_t *matmgr_init(ubo_binding ubo);
void matmgr_deinit(matmgr_t *mgr);

material_id matmgr_add_material(matmgr_t *mgr, struct MaterialDesc *mat_desc,
				shader_id shader);

void matmgr_buffer_mat(matmgr_t *mgr, ubomgr_t *ubomgr, material_id id);

#endif
