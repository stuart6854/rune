#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_norm;

layout(location = 0) out vec3 out_fragPos;
layout(location = 1) out vec2 out_uv;
layout(location = 2) out vec3 out_norm;

layout(std140, binding = 0) uniform Renderer
{
	mat4 worldMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
} u_renderer;

void main()
{
	out_fragPos = vec3(u_renderer.worldMatrix * vec4(a_pos, 1.0));
	out_uv = a_uv;
	out_norm = mat3(transpose(inverse(u_renderer.worldMatrix))) * a_norm;

	gl_Position = u_renderer.projMatrix * u_renderer.viewMatrix * vec4(out_fragPos, 1.0);
}