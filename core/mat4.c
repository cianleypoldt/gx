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
        float rx = quat_rotation[0];
        float ry = quat_rotation[1];
        float rz = quat_rotation[2];
        float rw = quat_rotation[3];

        // Build rotation matrix in row-major layout
        float rot_mat[16] = {
                1 - 2 * (ry * ry + rz * rz), 2 * (rx * ry - rw * rz), 2 * (rx * rz + rw * ry), 0,
                2 * (rx * ry + rw * rz), 1 - 2 * (rx * rx + rz * rz), 2 * (ry * rz - rw * rx), 0,
                2 * (rx * rz - rw * ry), 2 * (ry * rz + rw * rx), 1 - 2 * (rx * rx + ry * ry), 0,
                0, 0, 0, 1
        };

        // Negative position for view translation
        float tx = -vec3_position[0];
        float ty = -vec3_position[1];
        float tz = -vec3_position[2];

        // Transpose into column-major layout for GLSL,
        // and compute the translated fourth column as rot * t.
        // The fourth row of rot_mat is [0,0,0,1], so row==3 yields [0,0,0,1]
        // which correctly places 1.0 at m[3][3].
        for (int row = 0; row < 4; row++) {
                target_mat4[0 * 4 + row] = rot_mat[row * 4 + 0];
                target_mat4[1 * 4 + row] = rot_mat[row * 4 + 1];
                target_mat4[2 * 4 + row] = rot_mat[row * 4 + 2];
                target_mat4[3 * 4 + row] = rot_mat[row * 4 + 0] * tx +
                                           rot_mat[row * 4 + 1] * ty +
                                           rot_mat[row * 4 + 2] * tz +
                                           rot_mat[row * 4 + 3];  // w component: 0 for rows 0-2, 1 for row 3
        }
}

void compute_projection(float* mat4_target,
                        float  fov,
                        float  aspect,
                        float  near_plane,
                        float  far_plane) {
        /* Convert FOV from degrees to radians before using tanf.
           Many callers provide FOV in degrees (e.g. 45.0f), so
           applying the conversion ensures the projection is correct. */
        float rad = fov * (3.14159265358979323846f / 180.0f);
        float f   = 1.0f / tanf(rad * 0.5f);
        float fn  = far_plane - near_plane;

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

        // rot_mat defined row-major (row * 4 + col)
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

        // Write rotation into column-major layout:
        // target_mat4[col * 4 + row] = rot_mat[row * 4 + col]
        for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                        target_mat4[col * 4 + row] = rot_mat[row * 4 + col];
                }
        }

        // Set translation in the 4th column (column-major): indices 12,13,14
        target_mat4[12] = vec3_position[0];
        target_mat4[13] = vec3_position[1];
        target_mat4[14] = vec3_position[2];

        // Set bottom row (row index 3) across columns: indices 3,7,11,15
        target_mat4[3]  = 0.0f;
        target_mat4[7]  = 0.0f;
        target_mat4[11] = 0.0f;
        target_mat4[15] = 1.0f;
}
