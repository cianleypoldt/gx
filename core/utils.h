#ifndef GX_UTILS_H
#define GX_UTILS_H
/* Internal utilities */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int u32;
typedef double       f64;
typedef float        f32;

// array.c
#define GX_ARRAY_BASE_COUNT    16
#define GX_ARRAY_RESIZE_FACTOR 1.5

typedef struct {
        void*  data;
        size_t count;
        size_t capacity;
        size_t element_size;
} Array;

Array array_create(size_t element_size);
void  array_resize(Array* array, size_t capacity);
void  array_append(Array* array, void* data);
void* array_at(Array* array, size_t index);
void  array_delete(Array* array);

// platform.c
int   init_glfw();
void* create_window(int width, int height, const char* title);
int   init_OpenGL(void* window_ptr);
void  destroy_window(void* window_ptr);
void  terminate_glfw();

// file.c
typedef struct {
        void* buffer;
        long  size;
} file;

file load_file(const char* path);
void free_file(file f);

#endif
