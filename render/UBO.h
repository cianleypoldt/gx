#ifndef UBO_MGR_H
#define UBO_MGR_H

#include "common/def.h"
#include <sys/types.h>

typedef index_t ubo_binding;
typedef struct UBO_Manager ubomgr_t;

ubomgr_t *ubomgr_init();
void ubomgr_deinit(ubomgr_t *ubomgr);

ubo_binding ubomgr_add_ubo(ubomgr_t *mgr, size_t size);
void ubomgr_remove_ubo(ubomgr_t *mgr, ubo_binding index);

void ubomgr_update_ubo(ubomgr_t *mgr, ubo_binding index, const void *data);
void ubomgr_update_ubo_range(ubomgr_t *mgr, ubo_binding index, const void *data,
			     size_t offset, size_t size);

#endif
