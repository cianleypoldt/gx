#include <math.h>

void assign_identity(float* target_mat4) {
        for (int i = 0; i < 16; i++) {
                target_mat4[i] = 0.0f;
        }
        target_mat4[0]  = 1.0f;
        target_mat4[5]  = 1.0f;
        target_mat4[10] = 1.0f;
        target_mat4[15] = 1.0f;
}

void compute_view(float* target_mat4,
                  float* vec3_position,
                  float* quat_rotation) {
        // Convert quaternion to rotation matrix
        float rx = quat_rotation[0];
        float ry = quat_rotation[1];
        float rz = quat_rotation[2];
        float rw = quat_rotation[3];

        float rot_mat[16] = { 1 - 2 * (ry * ry + rz * rz),
                              2 * (rx * ry - rw * rz),
                              2 * (rx * rz + rw * ry),
                              0,
                              2 * (rx * ry + rw * rz),
                              1 - 2 * (rx * rx + rz * rz),
                              2 * (ry * rz - rw * rx),
                              0,
                              2 * (rx * rz - rw * ry),
                              2 * (ry * rz + rw * rx),
                              1 - 2 * (rx * rx + ry * ry),
                              0,
                              0,
                              0,
                              0,
                              1 };

        // Create translation matrix
        float tx = -vec3_position[0];
        float ty = -vec3_position[1];
        float tz = -vec3_position[2];

        // Multiply rotation by translation: view = rot * trans
        for (int i = 0; i < 4; i++) {
                target_mat4[i * 4 + 0] = rot_mat[i * 4 + 0];
                target_mat4[i * 4 + 1] = rot_mat[i * 4 + 1];
                target_mat4[i * 4 + 2] = rot_mat[i * 4 + 2];
                target_mat4[i * 4 + 3] = rot_mat[i * 4 + 0] * tx +
                                         rot_mat[i * 4 + 1] * ty +
                                         rot_mat[i * 4 + 2] * tz;
        }
}

void compute_projection(float* mat4_target,
                        float  fov,
                        float  aspect,
                        float  near_plane,
                        float  far_plane) {
        float f  = 1.0f / tanf(fov * 0.5f);
        float fn = far_plane - near_plane;

        // Initialize to zero
        for (int i = 0; i < 16; i++) {
                mat4_target[i] = 0.0f;
        }

        mat4_target[0]  = f / aspect;
        mat4_target[5]  = f;
        mat4_target[10] = -(far_plane + near_plane) / fn;
        mat4_target[11] = -1.0f;
        mat4_target[14] = -(2.0f * far_plane * near_plane) / fn;
}

void compute_transform(float* target_mat4,
                       float* vec3_position,
                       float* quat_rotation) {
        float rx = quat_rotation[0];
        float ry = quat_rotation[1];
        float rz = quat_rotation[2];
        float rw = quat_rotation[3];

        float rot_mat[16] = { 1 - 2 * (ry * ry + rz * rz),
                              2 * (rx * ry - rw * rz),
                              2 * (rx * rz + rw * ry),
                              0,
                              2 * (rx * ry + rw * rz),
                              1 - 2 * (rx * rx + rz * rz),
                              2 * (ry * rz - rw * rx),
                              0,
                              2 * (rx * rz - rw * ry),
                              2 * (ry * rz + rw * rx),
                              1 - 2 * (rx * rx + ry * ry),
                              0,
                              0,
                              0,
                              0,
                              1 };

        // Set rotation part
        for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                        target_mat4[i * 4 + j] = rot_mat[i * 4 + j];
                }
        }

        // Set translation part
        target_mat4[3]  = vec3_position[0];
        target_mat4[7]  = vec3_position[1];
        target_mat4[11] = vec3_position[2];

        // Set bottom row
        target_mat4[12] = 0.0f;
        target_mat4[13] = 0.0f;
        target_mat4[14] = 0.0f;
        target_mat4[15] = 1.0f;
}
