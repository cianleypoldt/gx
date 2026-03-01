#include "core/tools/slotmap.h"
#include "core/utils.h"
#include "gx.h"
#include "renderer/objects.h"
#include <stdlib.h>
#include <string.h>

sm_id_t gx_layout_create(gx_ctx *ctx, gx_layout_attrib *layout_desc,
			 unsigned int attribute_count, size_t stride)
{
	void *attrib_ptr = malloc(attribute_count * sizeof(gx_layout_attrib));
	memcpy(attrib_ptr, layout_desc,
	       attribute_count * sizeof(gx_layout_attrib));
	struct LayoutTemplate layout = { .attributes = attrib_ptr,
					 .attrib_count = attribute_count,
					 .stride = stride };
	return sm_add(ctx->glob_resources.layouts, &layout);
}
