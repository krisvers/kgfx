#version 450

layout (location = 0) in vec2 v_uv;

layout (location = 0) out vec4 out_color;

layout (binding = 1) uniform sampler2D unif_sampler;

void main() {
	out_color = texture(unif_sampler, v_uv);
}