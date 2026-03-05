#ifndef FILE_H
#define FILE_H

void assign_identity(float *target_mat4);
void compute_view(float *target_mat4, float *vec3_position,
		  float *quat_rotation);

void compute_projection(float *mat4_target, float fov, float aspect,
			float near_plane, float far_plane);

void compute_transform(float *target_mat4, float *vec3_position,
		       float *quat_rotation);

#endif
