#ifndef SLOTMAP_H
#define SLOTMAP_H

#include "def.h"

typedef size_t gen_t;

#define SM_INVALID_INDEX (index_t)(-1)
#define SM_INVALID_GENERATION (gen_t)(-1)

typedef struct sm_id_t sm_id_t;

struct sm_id_t {
	index_t map_index;
	gen_t gen;
};

typedef struct SlotMap slot_map_t;

slot_map_t *sm_create(size_t element_size);
void sm_delete(slot_map_t *sm);
int sm_id_exists(const slot_map_t *sm, sm_id_t id);
index_t sm_get_index(const slot_map_t *sm, sm_id_t id);
void *sm_at_id(const slot_map_t *sm, sm_id_t id);
void *sm_at_index(const slot_map_t *sm, index_t index);
index_t sm_dense_length(const slot_map_t *sm);
void sm_swap_elements(slot_map_t *sm, sm_id_t id_a, sm_id_t id_b);
sm_id_t sm_add(slot_map_t *sm, const void *data);
void sm_remove_id(slot_map_t *sm, sm_id_t id);

sm_id_t sm_invalid_id();

#endif
