#version 450

layout (location = 0) in vec3 v_color;

layout (location = 0) out vec4 out_color;

layout (binding = 1) uniform sampler2D unif_sampler;

void main() {
	out_color = texture(unif_sampler, v_color.xy);
}