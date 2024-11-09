#version 450

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUV;

layout (location = 0) out vec3 vertexColor;
layout (location = 1) out vec2 vertexUV;

layout (binding = 0) uniform UniformData {
    mat4x4 mvp;
} uniformBuffer;

void main() {
    gl_Position = uniformBuffer.mvp * vec4(inPosition, 1.0);
    vertexColor = inColor;
    vertexUV = inUV;
}
