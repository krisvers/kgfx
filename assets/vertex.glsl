#version 450

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;

layout (location = 0) out vec2 v_uv;

layout (binding = 0) uniform ubo_t {
	mat4 mvp;
} ubo;

void main() {
	gl_Position = ubo.mvp * vec4(in_pos, 1.0);
	v_uv = in_uv;
}