#include "glad/glad.h"
#include "GLFW/glfw3.h"

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
}

void* create_window(int height, int width, const char* title) {
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
}

void destroy_window(void* window_ptr) {
        if (window_ptr != NULL) {
                glfwDestroyWindow(window_ptr);
        }
}

void terminate_glfw() {
        glfwTerminate();
}
