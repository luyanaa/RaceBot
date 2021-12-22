#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS{
	vec3 Pos;
	vec2 TexCoord;
} gs_in[];

out GS{
	vec3 Pos;
	vec3 Normal;
	vec2 TexCoord;
} gs_out;



void main()
{
	vec3 norm=normalize(cross(gs_in[1].Pos-gs_in[0].Pos, gs_in[2].Pos-gs_in[1].Pos));

	gl_Position = gl_in[0].gl_Position;
	gs_out.Pos = gs_in[0].Pos;
	gs_out.Normal = norm;
	gs_out.TexCoord = gs_in[0].TexCoord;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	gs_out.Pos = gs_in[1].Pos;
	gs_out.Normal = norm;
	gs_out.TexCoord = gs_in[1].TexCoord;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	gs_out.Pos = gs_in[2].Pos;
	gs_out.Normal = norm;
	gs_out.TexCoord = gs_in[2].TexCoord;
	EmitVertex();

    EndPrimitive();
}