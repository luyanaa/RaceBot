#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 TexCoord;

out VS{
	vec3 Pos;
	vec2 TexCoord;
} vsOut;
uniform mat4 camera;
uniform mat4 trans;

void main()
{
	gl_Position = camera * trans * vec4(vertexPos, 1.0);
	vsOut.Pos = vertexPos;
	vsOut.TexCoord = vec2(TexCoord.x, TexCoord.y);
}