// init, window, input
#include "gx.h"

#include "core/utils.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        GLFWwindow* window_ptr;
        int         frame_width;
        int         frame_height;
} gx_ctx;

// Pläne:
//      1. Universal anwendbare Grundstruktur für Shaderkompilierung
//              -> Am wichtigsten ist die funktion für char* -> Kompilierter Shader
//              ->
//      2.

gx_ctx* gx_ctx_init(int frame_width, int frame_height) {
        init_glfw();
        GLFWwindow* window_ptr =
                create_window(frame_width, frame_height, "gx_viewport");

        init_OpenGL(window_ptr);

        gx_ctx* ctx = malloc(sizeof(gx_ctx));
        memset(ctx, 0, sizeof(gx_ctx));
        ctx->window_ptr   = window_ptr;
        ctx->frame_height = frame_height;
        ctx->frame_width  = frame_width;

        return ctx;
}

void gx_ctx_drop(gx_ctx* ctx) {
        if (ctx == NULL) {
                return;
        }
        destroy_window(ctx->window_ptr);
        terminate_glfw();
        free(ctx);
}
