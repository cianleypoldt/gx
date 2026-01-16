#ifndef RENDER_H
#define RENDER_H
#include "gx.h"

void init_camera(gx_ctx* ctx);
void init_mesh_data_ubo(gx_ctx* ctx);
void destroy_mesh_data_ubo(gx_ctx* ctx);

#endif
