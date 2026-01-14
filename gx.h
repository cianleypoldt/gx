#ifndef GX_H
#define GX_H

// plattform
typedef struct gx_ctx gx_ctx;

gx_ctx* gx_ctx_init(int frame_width, int frame_height);
void    gx_ctx_drop(gx_ctx* ctx);

typedef struct {
        unsigned int id;
} shader_program;

shader_program gx_create_shader(gx_ctx*     ctx,
                                const char* vertex_shader_path,
                                const char* fragment_shader_path);

#endif
