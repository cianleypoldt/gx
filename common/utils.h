#ifndef GX_UTILS_H
#define GX_UTILS_H

#include <stddef.h>

#include "dynamic_array.h"
#include "freelist.h"
#include "slot_map.h"

typedef unsigned int u32;
typedef double f64;
typedef float f32;

// platform.c
int glfw_init();
void glfw_deinit();
void *window_create(int width, int height, const char *title);
void window_destroy(void *window_ptr);
int glad_init(void *window_ptr);

// file.c
typedef struct {
	void *buffer;
	long size;
} file;

file load_file(const char *path);
void free_file(file f);

// mat4.c
void assign_identity(float *target_mat4);
void compute_view(float *target_mat4, float *vec3_position,
		  float *quat_rotation);

void compute_projection(float *mat4_target, float fov, float aspect,
			float near_plane, float far_plane);

void compute_transform(float *target_mat4, float *vec3_position,
		       float *quat_rotation);

#endif
