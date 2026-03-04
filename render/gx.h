#ifndef RENDERER_H
#define RENDERER_H

typedef struct GX_Context gxctx;

gxctx *gx_init();
void gx_deinit(gxctx *renderer);

#endif
