#version 150

in vec3 position;
in vec3 normal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float animationOffset;

out vec3 color;

void main()
{

	vec3 lightV = normalize(vec3(1,-1,0));
	vec3 mvNorm = vec3(model * vec4(normal, 0));
	float diffuse = max(dot(mvNorm, lightV), 0.3);

	vec3 startColor = vec3(1.0, 0.5, 0.0);
	color = startColor * diffuse;

	vec3 newPos = position + vec3(0, 0, (sin((animationOffset + position.x) * 4) / 16));
	gl_Position = proj * view * model * vec4(newPos, 1.0);

}