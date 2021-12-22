#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS{
	vec3 Color;
	vec3 Pos;
	vec3 Normal;
} gs_in[];

out GS{
	vec3 Color;
	vec3 Pos;
	vec3 Normal;
} gs_out;

void main()
{
	vec3 norm=normalize(gs_in[0].Normal+gs_in[1].Normal+gs_in[2].Normal);

	gl_Position = gl_in[0].gl_Position;
	gs_out.Pos = gs_in[0].Pos;
	gs_out.Normal = gs_in[0].Normal;
	gs_out.Color = gs_in[0].Color;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	gs_out.Pos = gs_in[1].Pos;
	gs_out.Normal = gs_in[1].Normal;
	gs_out.Color = gs_in[1].Color;
	EmitVertex();
	
	gl_Position = gl_in[2].gl_Position;
	gs_out.Pos = gs_in[2].Pos;
	gs_out.Normal = gs_in[2].Normal;
	gs_out.Color = gs_in[2].Color;
	EmitVertex();

    EndPrimitive();
}