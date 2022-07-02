#version 450 core

layout(location = 0) in vec2 in_uv;

layout(binding = 0) uniform sampler2D tex;

layout (location = 0) out vec4 out_fragColor;

void main()
{
	vec4 color = texture(tex, in_uv);
	out_fragColor = color; //vec4(1.0, 0.5, 0.2, 1.0);
}