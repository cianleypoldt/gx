#ifndef MESH_BUFFER_MGR_H
#define MESH_BUFFER_MGR_H

#include "common/slot_map_id.h"
#include "render/public.h"
#include <sys/types.h>

#define GX_MAX_LAYOUT_ATTRIBUTES 64

typedef sm_id_t mesh_buffer_id;
typedef index_t layout_index;
typedef struct MeshBufferManager mbmgr_t;

mbmgr_t *mbmgr_init();
void mbmgr_deinit(mbmgr_t *ubomgr);

layout_index mbmgr_add_layout(mbmgr_t *mgr, struct LayoutAttribute *attributes,
			      index_t attrib_count, size_t stride);

mesh_buffer_id mbmgr_create_buffer(mbmgr_t *mgr, layout_index vertex_layout,
				   void *vertex_data, size_t vertex_data_size,
				   void *index_data, index_t index_count);

void mbmgr_destroy_buffer(mbmgr_t *mgr, mesh_buffer_id id);

#endif
