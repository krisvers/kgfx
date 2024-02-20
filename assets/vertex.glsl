#version 450

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec3 in_color;

layout (location = 0) out vec3 v_color;

layout (binding = 0) uniform ubo_t {
	mat4 mvp;
} ubo;

void main() {
	gl_Position = ubo.mvp * vec4(in_pos, 0.0, 1.0);
	v_color = in_color;
}