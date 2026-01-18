#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "gx.h"
#include "renderer/objects.h"

#include <stdio.h>

int init_glfw() {
        if (!glfwInit()) {
                fprintf(stderr, "Failed to initialize GLFW\n");
                return 0;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        return 0;
}

void* create_window(int width, int height, const char* title) {
        GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window == NULL) {
                fprintf(stderr, "Failed to create GLFW window\n");
                return NULL;
        }
        glfwMakeContextCurrent(window);
        return window;
}

int init_OpenGL(void* window_ptr) {
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                fprintf(stderr, "Failed to initialize GLAD\n");
                glfwDestroyWindow(window_ptr);
                glfwTerminate();
                return 0;
        }
        glEnable(GL_DEPTH_TEST);
        return 1;
}

void gx_clear() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gx_present(gx_ctx* ctx) {
        glfwSwapBuffers(ctx->window_ptr);
}

int gx_should_close(gx_ctx* ctx) {
        glfwPollEvents();
        if (glfwGetKey(ctx->window_ptr, GLFW_KEY_Q) == GLFW_PRESS) {
                return 1;
        }
        return glfwWindowShouldClose(ctx->window_ptr);
}

void destroy_window(void* window_ptr) {
        if (window_ptr != NULL) {
                glfwDestroyWindow(window_ptr);
        }
}

void terminate_glfw() {
        glfwTerminate();
}
