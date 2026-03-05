#ifndef PLATFORM_H
#define PLATFORM_H

int glfw_init();
void glfw_deinit();
void *window_create(int width, int height, const char *title);
void window_destroy(void *window_ptr);
int glad_init(void *window_ptr);

#endif
