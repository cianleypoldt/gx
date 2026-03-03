#ifndef UBO_MGR_H
#define UBO_MGR_H

#include "common/slot_map.h"
#include <sys/types.h>

typedef sm_id_t shader_id;
typedef struct ShaderManager shadermgr_t;

shadermgr_t *shadermgr_create();
void shadermgr_destroy(shadermgr_t *ubomgr);

shader_id shadermgr_create_program(shadermgr_t *mgr, char *vs_src,
				   size_t vs_src_size, char *fs_src,
				   size_t fs_src_size);
void shadermgr_destroy_program(shadermgr_t *mgr, shader_id id);

void

#endif
