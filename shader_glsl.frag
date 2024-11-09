#version 450

layout (location = 0) in vec3 vertexColor;
layout (location = 1) in vec2 vertexUV;

layout (location = 0) out vec4 outColor;

layout (binding = 1) uniform texture2D uniformTexture;
layout (binding = 2) uniform sampler uniformSampler;

void main() {
    vec4 color = texture(sampler2D(uniformTexture, uniformSampler), vertexUV);
    outColor = color;
}
