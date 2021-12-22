#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;

out VS{
	vec3 Color;
	vec3 Pos;
	vec3 Normal;
} vsOut;
uniform mat4 camera;

void main()
{
	gl_Position = camera * vec4(vertexPos, 1.0f);
	vsOut.Pos = vertexPos;
	vsOut.Color = vec3(1.0f,0.5f,0.0f);
	vsOut.Normal = vertexNormal;
}