#include "core/utils.h"
#include "gx.h"
#include "renderer/objects.h"

gx_layout gx_layout_create(gx_ctx*           ctx,
                           gx_layout_attrib* layout_desc,
                           unsigned int      attribute_count,
                           size_t            stride) {
        void* attrib_ptr = malloc(attribute_count * sizeof(gx_layout_attrib));
        struct LayoutTemplate layout = { .attributes   = attrib_ptr,
                                         .attrib_count = attribute_count,
                                         .stride       = stride };
        array_append(&ctx->glob_resources.layouts, &layout);
        return ctx->glob_resources.layouts.count - 1;
}
