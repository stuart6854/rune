#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_norm;

layout(location = 0) out vec2 out_uv;

layout(std140, binding = 0) uniform UniformBlock
{
	mat4 mvp;
	float test;
} u_uniforms;

void main()
{
	out_uv = a_uv;
	gl_Position = u_uniforms.mvp * vec4(a_pos, 1.0);
}