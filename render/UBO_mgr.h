#ifndef UBO_MGR_H
#define UBO_MGR_H

#include "common/slot_map.h"
#include <sys/types.h>

typedef sm_id_t ubo_id;
typedef struct UBO_Manager ubomgr_t;

ubomgr_t *shader_program_create(const void *);
void ubomgr_destroy(ubomgr_t *ubomgr);

ubo_id ubomgr_create_ubo(ubomgr_t *mgr, size_t size);
void ubomgr_destroy_ubo(ubomgr_t *mgr, ubo_id id);

void ubomgr_update_ubo(ubomgr_t *mgr, ubo_id id, const void *data);
void ubomgr_update_ubo_range(ubomgr_t *mgr, ubo_id id, const void *data,
			     size_t offset, size_t size);

#endif
