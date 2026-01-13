// init, window, input
#include "external/glad/glad.h"
#include "GLFW/glfw3.h"
#include "gx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pläne:
//      1. Universal anwendbare Grundstruktur für Shaderkompilierung
//              -> Am wichtigsten ist die funktion für char* -> Kompilierter Shader
//              ->
//      2.

gx_ctx* gx_ctx_init(int frame_width, int frame_height) {
        if (!glfwInit()) {
                fprintf(stderr, "Failed to initialize GLFW\n");
                return NULL;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow* window = glfwCreateWindow(frame_width, frame_height,
                                              "gx_viewport", NULL, NULL);

        if (window == NULL) {
                fprintf(stderr, "Failed to create GLFW window\n");
                glfwTerminate();
                return NULL;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                fprintf(stderr, "Failed to initialize GLAD\n");
                glfwDestroyWindow(window);
                glfwTerminate();
                return NULL;
        }

        gx_ctx* ctx = malloc(sizeof(gx_ctx));
        memset(ctx, 0, sizeof(gx_ctx));
        ctx->window_ptr   = window;
        ctx->frame_height = frame_height;
        ctx->frame_width  = frame_width;

        return ctx;
}

void gx_ctx_drop(gx_ctx* ctx) {
        if (ctx == NULL) {
                return;
        }

        if (ctx->window_ptr != NULL) {
                glfwDestroyWindow(ctx->window_ptr);
        }
        glfwTerminate();
        free(ctx);
}
