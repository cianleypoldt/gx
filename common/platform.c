#include "third_party/glad/glad.h"
#include "GLFW/glfw3.h"

#include "platform.h"

#include <stdio.h>

int glfw_init()
{
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

void *window_create(int width, int height, const char *title)
{
	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to create GLFW window\n");
		return NULL;
	}
	glfwMakeContextCurrent(window);
	return window;
}

int glad_init(void *window_ptr)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to initialize GLAD\n");
		glfwDestroyWindow(window_ptr);
		glfwTerminate();
		return 0;
	}
	glEnable(GL_DEPTH_TEST);
	return 1;
}

void window_destroy(void *window_ptr)
{
	glfwDestroyWindow(window_ptr);
}

void glfw_deinit()
{
	glfwTerminate();
}
