#version 430 core

in vec2 UV;

out float ao;

uniform sampler2D aoTex;

uniform vec2 windowScale;


// -4 4
// 81

//-2 2
// 25

void main()
{
	float sum = 0.0;
    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++)
            sum += (texture(aoTex, UV + 2.0 * vec2(windowScale.x * x, windowScale.y * y)).r) / 9.0;//25.0;
	ao = sum;

  //  ao = (texture(aoTex, UV).r); 
}
