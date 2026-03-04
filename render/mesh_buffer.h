#ifndef MESH_BUFFER_MGR_H
#define MESH_BUFFER_MGR_H

#include "common/slot_map.h"
#include <sys/types.h>

typedef sm_id_t mesh_buffer_id;
typedef struct MeshBufferManager mbmgr_t;

mbmgr_t *mbmgr_create();
void mbmgr_destroy(mbmgr_t *ubomgr);

mesh_buffer_id mbmgr_create_buffer(mbmgr_t *mgr, void *vertex_data,
				   size_t vertex_data_size, void *index_data,
				   size_t index_data_size);

void mbmgr_destroy_buffer(mbmgr_t *mgr, mesh_buffer_id id);

void mbmgr_update_buffer(mbmgr_t *mgr, mesh_buffer_id id, const void *data);

#endif
