#version 460 core

layout(location = 0) in vec3 aPos;

out vec3 frag_color;

layout(std140, binding = 0) uniform CameraData {
    mat4 proj;
    mat4 view;
} camera;

layout(std140, binding = 1) uniform PerMeshData {
    mat4 transform;
} mesh;

void main() {
    gl_Position = camera.proj * camera.view * mesh.transform * vec4(aPos, 1.0);
    frag_color = vec3(1.0, 0.0, 0.0);
}
