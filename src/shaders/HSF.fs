#version 330 core

in vec2 frg_vi;
in vec3 frg_norm;
in vec3 frg_pos;
in float w;
in float z;
in vec4 col;


layout(location = 0) out vec4 color;
layout(location = 1) out vec4 pos;
layout(location = 2) out vec3 norm;

//layout(std140) uniform hueList
//{
  //float hueListData[50];
//};

uniform float hsList[50];


//HSV_RGB
//vec3 hsv2rgb(vec3 c)
//{
//    c = clamp(c, 0, 1);
//    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
//}

void main()
{
    //color = vec4(hsv2rgb(vec3(hueList[int(round(frg_svh.z))], frg_svh.xy)), 0);
    int indx = 2 * int(round(frg_vi.y));
    color = vec4(hsList[indx], hsList[indx+1], frg_vi.x, 0);
   // color = col;
	pos = vec4(frg_pos, z / w); // z/w
	norm = normalize(frg_norm);
}