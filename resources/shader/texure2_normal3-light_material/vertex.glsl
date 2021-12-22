#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 vertexNormal;

out VS{
	vec3 Pos;
	vec2 TexCoord;
	vec3 Normal;
} vsOut;

uniform mat4 camera;
uniform mat4 trans;

void main()
{
	gl_Position = camera * trans * vec4(vertexPos, 1.0);
	vsOut.Pos =vec3( trans * vec4(vertexPos, 1.0));
	vsOut.TexCoord = vec2(TexCoord.x, TexCoord.y);
	vsOut.Normal = mat3(transpose(inverse(trans))) * vertexNormal;

}
