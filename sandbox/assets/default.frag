#version 450 core

layout(location = 0) in vec3 in_fragPos;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_norm;

layout (location = 0) out vec4 out_fragColor;

struct Light
{
	vec3 position;
	int isDirectional;
	vec4 direction;

	float constant;
	float linear;
	float quadratic;

	vec4 diffuse;
	vec4 specular;
};

#define MAX_LIGHTS 32
layout(std140, binding = 1) uniform Lighting
{
	vec4 viewPos;
	vec3 ambient;

	int lightCount;
	Light lights[MAX_LIGHTS];
} u_lighting;

layout(std140, binding = 2) uniform Material
{
	vec3 diffuse;
	float shininess;
} u_material;

layout(binding = 3) uniform sampler2D tex;

vec3 calcLight(Light light, vec3 objectColor, vec3 fragNormal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = vec3(0.0);
	lightDir += normalize(light.position.xyz - fragPos) * (1 - light.isDirectional);
	lightDir += normalize(light.direction.xyz) * light.isDirectional;
	// Diffuse shading
	float diff = max(dot(fragNormal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	// Combine results
	vec3 ambient = u_lighting.ambient * objectColor;
	vec3 diffuse = light.diffuse.xyz * diff * objectColor;
	vec3 specular = light.specular.xyz * spec;
	return ambient + diffuse;//  + specular;
}

void main()
{
	vec3 objectColor = texture(tex, in_uv).xyz;
	vec3 surfaceNormal = normalize(in_norm);
	vec3 viewDir = normalize(u_lighting.viewPos.xyz - in_fragPos);

	vec3 color = vec3(0.0);
	for(int i = 0; i < u_lighting.lightCount; ++i)
	{
		color += calcLight(u_lighting.lights[i], objectColor, surfaceNormal, in_fragPos, viewDir);
	}

	// out_fragColor = vec4(u_lighting.ambient * objectColor, 1.0);
	out_fragColor = vec4(color, 1.0);
	// out_fragColor = vec4(u_lighting.ambient, 1.0);
}