#version 460 core

uniform int time;

layout(std140, binding = 0) uniform CameraUBO {
    mat4 proj;
    mat4 view;
} camera;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;

out vec3 frag_color;

void main() {
    gl_Position = time * camera.proj * camera.view * vec4(aPos, 1.0);
    frag_color = aCol;
}
