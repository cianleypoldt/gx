#ifndef GX_UTILS_H
#define GX_UTILS_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define GX_ARRAY_BASE_COUNT    16
#define GX_ARRAY_RESIZE_FACTOR 1.5

typedef struct {
        void*  data;
        size_t count;
        size_t capacity;
        size_t element_size;
} array;

// array.c
array gx_array_create(size_t element_size);
void  gx_array_resize(array* array, size_t capacity);
void  gx_array_appand(array* array, void* data);
void* gx_array_at(array* array, size_t index);
void  gx_array_delete(array* array);

// platform.c
int   init_glfw();
void* create_window(int width, int height, const char* title);
int   init_OpenGL(void* window_ptr);
void  destroy_window(void* window_ptr);
void  terminate_glfw();
#endif
