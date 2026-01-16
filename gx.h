#ifndef GX_H
#define GX_H
#include <stddef.h>
typedef struct gx_ctx gx_ctx;
typedef unsigned int  gx_layout;
typedef unsigned int  gx_shader;
typedef unsigned int  gx_camera;
typedef unsigned int  gx_mesh;

gx_ctx* gx_ctx_init(int frame_width, int frame_height);
void    gx_ctx_drop(gx_ctx* ctx);

void gx_clear(gx_ctx* ctx);
void gx_present(gx_ctx* ctx);
int  gx_should_close(gx_ctx* ctx);

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

gx_camera gx_camera_create(gx_ctx* ctx, float fov);
void      gx_camera_delete(gx_ctx* ctx, gx_camera camera);

void gx_camera_set_position(gx_ctx* ctx, float position[3]);
void gx_camera_set_rotation(gx_ctx* ctx, float quat_rotation[3]);

typedef struct {
        gx_layout layout;

        void*  vertex_data;
        size_t vertex_data_size;
        void*  index_data;
        size_t index_data_size;
        size_t index_count;
} gx_mesh_desc;

gx_mesh gx_mesh_create(gx_ctx* ctx, gx_mesh_desc mesh_desc);
void    gx_mesh_delete(gx_ctx* ctx, gx_mesh mesh);

void gx_mesh_set_position(gx_ctx* ctx, gx_mesh mesh, float position[3]);
void gx_mesh_set_rotation(gx_ctx* ctx, gx_mesh mesh, float quat_rotation[4]);

void gx_mesh_render(gx_ctx* ctx, gx_mesh mesh, gx_shader shader);

void gx_render(gx_ctx* ctx);

#endif
