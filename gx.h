#ifndef GX_H
#define GX_H
#include <stddef.h>
typedef struct gx_ctx gx_ctx;
typedef unsigned int  gx_layout;
typedef unsigned int  gx_mesh;
typedef unsigned int  gx_shader;

gx_ctx* gx_ctx_init(int frame_width, int frame_height);
void    gx_ctx_drop(gx_ctx* ctx);

gx_shader gx_shader_create(gx_ctx*     ctx,
                           const char* vertex_shader_path,
                           const char* fragment_shader_path);

enum gx_layout_attrib_type { GX_TYPE_FLOAT, GX_TYPE_INT, GX_TYPE_UNSIGNED_INT };

typedef struct {
        enum gx_layout_attrib_type type;
        int                        count;
        int                        normalized;
        unsigned int               offset;
} gx_layout_attrib;

gx_layout gx_layout_create(gx_ctx*           ctx,
                           gx_layout_attrib* layout_desc,
                           unsigned int      attribute_count,
                           size_t            stride);

typedef struct {
        gx_layout layout;
        gx_shader shader;

        void*  vertex_data;
        size_t vertex_data_size;
        void*  index_data;
        size_t index_data_size;
        size_t index_count;
} gx_mesh_desc;

gx_mesh gx_mesh_create(gx_ctx* ctx, gx_mesh_desc mesh_desc);
void    gx_mesh_delete(gx_ctx* ctx, gx_mesh mesh);

void gx_render(gx_ctx* ctx);

#endif
