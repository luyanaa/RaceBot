#version 330 core

layout (location = 0) in vec3 vertexPos;

out VS{
	vec3 Color;
	vec3 Pos;
} vsOut;
uniform mat4 camera;
uniform mat4 trans;

void main()
{
	gl_Position = camera *trans* vec4(vertexPos, 1.0f);
	vsOut.Pos = vec3(trans * vec4(vertexPos, 1.0f));
	vsOut.Color = vec3(1.0f,1.0f,1.0f);
}