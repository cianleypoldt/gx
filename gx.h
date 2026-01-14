#ifndef GX_H
#define GX_H

#include "external/glad/glad.h"
#include "GLFW/glfw3.h"

// plattform
typedef struct gx_ctx gx_ctx;

gx_ctx* gx_ctx_init(int height, int width);
void    gx_ctx_drop(gx_ctx* ctx);

#endif
