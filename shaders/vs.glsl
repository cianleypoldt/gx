#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;

out vec3 frag_color;

layout(std140, binding = 0) uniform CameraData {
    mat4 proj;
    mat4 view;
} camera;

layout(std140, binding = 1) uniform PerMeshData {
    mat4 transform;
} mesh;

void main() {
    gl_Position =  vec4(aPos, 1.0);
    frag_color = vec3(aCol);
}
