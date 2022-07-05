#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_norm;

layout(location = 0) out vec2 out_uv;

layout(std140, binding = 0) uniform Renderer
{
	mat4 viewProj;
	// Lighting
	vec3 ambient;
} u_renderer;

layout(std140,binding = 1) uniform Material
{
	vec3 diffuse;
	float specular;
} u_material;

layout (std140, binding = 2) uniform Object
{
	mat4 model;
} u_object;

void main()
{
	out_uv = a_uv;
	gl_Position = u_renderer.viewProj * u_object.model * vec4(a_pos, 1.0);
}