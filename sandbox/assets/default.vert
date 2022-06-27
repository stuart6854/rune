#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_norm;

void main()
{
	gl_Position = vec4(a_pos, 1.0);
}