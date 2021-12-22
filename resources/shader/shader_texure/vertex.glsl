#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 camera;
uniform mat4 trans;

//out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = camera * trans *vec4(aPos, 1.0);
	//ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
