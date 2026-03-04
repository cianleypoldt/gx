#ifndef LAYOUT_DESC_H
#define LAYOUT_DESC_H

#include <stdint.h>

typedef struct {
	uint32_t index;

	int normalized;
	uint32_t offset;
} gx_attrib_desc;

typedef struct {
} gx__vertex_layout;

#endif
