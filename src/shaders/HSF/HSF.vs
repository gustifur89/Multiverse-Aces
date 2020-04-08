#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 vi;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 NM;

uniform float hsList[50];

out vec2 frg_vi;
out vec3 frg_norm;
out vec3 frg_pos;
out float w;
out float z;
out vec4 col;

void main () 
{
	vec4 tempPos = MVP * vec4(pos, 1);
	gl_Position = tempPos;
	frg_vi = vi;

//	int indx = 2 * int(round(vi.y));
//	col = vec4(hsList[indx], hsList[indx+1], vi.x, 0);

	//NM needs to send them to View space
	frg_norm = (NM * vec4(norm, 0)).xyz;
	frg_pos = vec3(MV * vec4(pos,1));
	z = tempPos.z;
	w = tempPos.w;
}