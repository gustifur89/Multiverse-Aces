#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 UV;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 NM;

out vec2 frg_UV;
out vec3 frg_norm;
out vec3 frg_pos;
out float z;
out float w;

void main () 
{
	vec4 tempPos = MVP * vec4(pos, 1);
	gl_Position = tempPos;
	frg_UV = UV;
	frg_norm = (NM * vec4(norm, 0)).xyz;
	frg_pos = vec3(NM * vec4(pos,1));
	z= tempPos.z;
	w = tempPos.w;
}
