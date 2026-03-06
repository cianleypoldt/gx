#ifndef RENDERER_H
#define RENDERER_H

#include "common/slot_map_id.h"
#include "render/public.h"

typedef struct GX_Context gxctx;
typedef index_t gx_layout;
typedef struct LayoutAttribute gx_layout_attribute;
typedef index_t gx_material;
typedef sm_id_t gx_mesh;
typedef sm_id_t gx_shader;

typedef struct Transform gx_transform_t;
typedef struct CameraParameters gx_camera_params;
typedef struct MaterialDesc gx_material_desc;

gxctx *gx_init(void);

void gx_deinit(gxctx *renderer);

gx_layout gx_add_layout(gxctx *ctx, gx_layout_attribute *attribs,
			index_t attrib_count, size_t stride);

gx_mesh gx_add_mesh(gxctx *ctx, gx_layout vertex_layout, void *vertex_data,
		    size_t vertex_data_size, void *index_data,
		    index_t index_count);

void gx_remove_mesh(gxctx *ctx, gx_mesh mesh);

gx_shader gx_add_shader(gxctx *ctx, char *vs_src, size_t vs_src_size,
			char *fs_src, size_t fs_src_size);

void gx_remove_shader(gxctx *ctx, gx_shader shader);

void gx_shader_print_uniforms(gxctx *ctx, gx_shader shader);

void gx_set_camera_transform(gxctx *ctx, gx_transform_t *transform);
void gx_set_camera_params(gxctx *ctx, gx_camera_params *params);

gx_material gx_add_material(gxctx *ctx, gx_material_desc *desc,
			    gx_shader shader);

#endif
