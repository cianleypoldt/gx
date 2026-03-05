#ifndef SHADER_H
#define SHADER_H

#include "common/slot_map_id.h"
#include <sys/types.h>

typedef sm_id_t shader_id;
typedef int uniform_id_t;
typedef struct ShaderManager shadermgr_t;

#define SHADERMGR_INVALID_UNIFORM_ID (uniform_id_t)(-1)

shadermgr_t *shadermgr_init();
void shadermgr_deinit(shadermgr_t *ubomgr);

shader_id shadermgr_add_program(shadermgr_t *mgr, char *vs_src,
				size_t vs_src_size, char *fs_src,
				size_t fs_src_size);
void shadermgr_remove_program(shadermgr_t *mgr, shader_id id);

uniform_id_t shadermgr_get_uniform(shadermgr_t *mgr, shader_id id,
				   const char *);

void shadermgr_log_uniforms(shadermgr_t *mgr, shader_id id);

#endif
