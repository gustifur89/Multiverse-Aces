#version 430 core
//430

in vec2 UV;

out vec4 color;

//uniform sampler2D renderedTexture;
uniform sampler2D colTex;
uniform sampler2D posTex;
uniform sampler2D normTex;
uniform sampler2DArray shadowMap; // sampler2DArray 
uniform sampler2D ssaoTex;

uniform int numShadows;

uniform vec2 windowScale;
uniform vec3 clearColor;
uniform float gamma;
uniform float exposure;
uniform vec3 globalLightDir;
uniform float globalLightInten;
uniform float ambient;
uniform int useShadow;
uniform int useSSAO;
uniform mat4 lsm;
uniform mat4 VSMat;
uniform mat4 invVSMat;
uniform vec2 sgfirohti;
uniform mat3 conMatX;
uniform mat3 conMatY;


#define LIGHT vec3(1, 1, 0)

struct Light {
	vec3 pos;
	float pad0;
	mat4 LightSpaceMatrix;
	float pad1;
	float radius;
	float intensity;
	float pad2;
//	vec3 color;
//	float pad3;
};

//std430 std140
layout(std140, binding = 2) buffer LightSSBO { 
	Light lights[]; 
};


float LinearizeDepth(float depth)
{
	float near_plane = 0.1;
	float far_plane = 60.0;
    float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}


float ShadowCalculation(vec4 fragPosLightSpace, float bias, int indx)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  
    projCoords = projCoords * 0.5 + 0.5;
   float closestDepth = texture(shadowMap, vec3(projCoords.xy, indx)).r; 
  
    float currentDepth = projCoords.z;
 
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	float adjust = (projCoords.z > 1.0 || projCoords.z < 0.0) ? 0.0 : 1.0;
    return (1 - shadow) * adjust;
}  



float lighting(vec3 pos, vec3 norm, int indx)
{
	//this assumes it is in world space....


	vec3 light_pos = (VSMat * vec4(lights[indx].pos, 1)).xyz;//view space
	float light_radius =  lights[indx].radius;				
	float light_intensity = lights[indx].intensity;			
	mat4 light_spaceMatrix = lights[indx].LightSpaceMatrix;	//View to Light space


	vec3 dP = light_pos - pos.xyz;
	float diffuse = clamp(dot(normalize(dP), norm), 0, 1);

	float relDistance = clamp((light_radius - length(dP)) / light_radius, 0, 1);

	float iten = relDistance;

	vec4 posInLS = light_spaceMatrix * vec4(pos.xyz, 1);

	float bias = max(0.000005 * (1.0 - dot(norm, normalize(dP))), 0.000001);

//	float shadow = ShadowCalculation(UV, 0, pos.xyz);       
	float shadow = ShadowCalculation(posInLS, bias, indx);    

	return shadow * diffuse * iten;
}

float globalLighting(vec3 norm)
{
	//float diffuse = clamp(dot(normalize((VSMat * vec4(globalLightDir,0)).xyz), norm), 0, 1);
	//float val = dot(normalize((VSMat * vec4(globalLightDir,0)).xyz), norm);
	//float dif = val * abs(val);
	//float diffuse = 0.5 * clamp(dif, -1, 1) + 0.5;
	float diffuse = 0.5 * clamp(dot(normalize((VSMat * vec4(globalLightDir,0)).xyz), norm), -1, 1) + 0.5;
	//float diffuse = 0.5 * dot(normalize((VSMat * vec4(globalLightDir,0)).xyz), normalize(norm)) + 0.5;
	return diffuse * globalLightInten;
}

float getAmbient()
{
//texture( ssaoTex, UV ).x; // 0.6
//	vec2 texelSize = vec2(1/800,1/600);   //1.0 / vec2(textureSize(ssaoInput, 0));
//    float result = 0.0;
//    for (int x = -2; x < 2; ++x) 
//    {
//        for (int y = -2; y < 2; ++y) 
//        {
//            vec2 offset = vec2(float(x), float(y)) * texelSize;
//            result += texture(ssaoTex, UV + offset).r;
//        }
//    }
 //  return result / (4.0 * 4.0);

//	float val = (useSSAO == 1 ? texture(ssaoTex, UV).r  : 1.0); 
	return texture(ssaoTex, UV).r;
}

float blendHue(float a, float b, float t)
{
	//float type0 = fabs(a - b);
	//float type0 = 

	float dif1 = a - b;
	float dif2 = 1 - a + b;
	
	float a_ = a;
	float b_ = b;

	if (abs(a-b) < abs(1 + b - a))
	{
		a_ = a;
		b_ = 1 + b;
	}

	return fract((1 - t) * a + (t) * b);


//	totalVector = [0, 0]
//for each angle:
//    vector = [cos(angle), sin(angle)]
//    totalVector += vector
//if length(totalVector) < aSmallNumber:
//    # error, angles are all over the place so there's no meaningful average
//avgAngle = atan2(totalVector.y, totalVector.x)

	//float out = 0.0;
	
	//if(dif1 < dif2)
	//{
	//	out = fract((1 - t) * a + (t) * (b));	
	//}
	//else
	//{
	//	out = fract((1 - t) * a + (t) * (b+1.0));
	//}
	//return out;
}

//RGB to HSV
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

//HSV_RGB
vec3 hsv2rgb(vec3 c)
{
    c = clamp(c, 0, 1);
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 blendHSV(vec3 a, vec3 b, float t)
{
	vec3 rgbA = hsv2rgb(a);
	vec3 rgbB = hsv2rgb(b);

	vec3 blend = (1 - t) * rgbA + (t) * rgbB;

	return rgb2hsv(blend);

	//vec3 o = vec3(0);
	//t = clamp(t, 0, 1);
	//t = t*t;
	//o.yz = (1 - t) * a.yz + (t) * b.yz;
	////do a fancy rotation thing on hue
	//o.x = blendHue(a.x, b.x, t);
	//
	////o.x = a.x;
	////o.z = a.z;
	//
	//return o;
}

void main()
{
	vec3 norm = texture( normTex, UV ).xyz;
	vec4 pos = texture( posTex, UV );
	vec4 col = texture( colTex, UV );
	float lightVal = ambient;


	lightVal += globalLighting(norm);

#ifdef USE_SHADOW
	for(int i=0;i<numShadows;i++)
	{
		lightVal += lighting(pos.xyz, norm, i);
	}
#endif

float sky = col.a;

	float ao = 1.0;
#ifdef USE_SSAO
	ao = getAmbient();
	lightVal *= ao;
	ao = (1.0-sky) * ao + sky;
#endif	
	

	//lightVal += lighting(pos.xyz, norm, 4);


	float brightness = (1-sky) * lightVal + (sky) * globalLightInten; //(col.a == 0 ? lightVal : globalLightInten);

    // Exposure tone mapping
	float mappedLight = 1.0 - exp(brightness * exposure);
    // Gamma correction 
    mappedLight = pow(mappedLight, 1.0 / gamma);

	//vec3 colShaded = vec3(col.rg, mappedLight * col.b);

	vec3 colShaded = vec3(col.rg, ao * mappedLight * col.b);

	
	
	//vec3 colShaded = vec3(col.rg, col.b * (col.a == 0 ? mappedLight : 1.0));
	//vec3 colShaded = vec3(col.rg, col.b * (col.a == 0 ?  pow(lightVal, gamma) : 1.0));

	vec2 nUV = UV * 2 - 1;
	vec4 skyPos = normalize(invVSMat * vec4(nUV.x, nUV.y, -1, 0));
	//vec4 skyPos = invVSMat * vec4(nUV.x, nUV.y, -1, 0);


	float minDistance = 1.0;
	float maxDistance = 1000.0;
	float distance = length(pos.xyz);
	float distanceFactor = clamp( 1 + (distance - minDistance) * (-1.0 / (maxDistance - minDistance)), 0, 1);

	//float distanceFactor = clamp((length(pos.xyz)- minDistance) / fallOffFactorDistance, 0, 1);

	//colShaded.g = ((sky)*(1 - skyPos.y) + (1 - sky)*(distanceFactor)) * colShaded.g;

	//vec3 skyColor = vec3(0.16, 0.3, 0.6);//clearColor;
	
	vec3 skyColor = clearColor;
	skyColor.g = (1 - skyPos.y) * clearColor.g;

	//colShaded = ((sky)*(1 - skyPos.y) + (1 - sky)*(distanceFactor)) * colShaded;
	
	colShaded = (sky == 1 ? skyColor : blendHSV(colShaded, skyColor, 1 - distanceFactor));//distanceFactor * colShaded + (1-distanceFactor) * skyColor;

	//colShaded.g = blend(clearColor.g, colShaded.g, distanceFactor);

	//colShaded = blend(clearColor, colShaded, distanceFactor);	

//	colShaded = clearColor;

	//colShaded.b = floor(colShaded.b * 5.0) / 5.0;

    color.rgb = hsv2rgb(colShaded);

#ifdef USE_SOBEL
	vec3 sumX = vec3(0.0);
	vec3 sumY = vec3(0.0);
    vec3 tempCol = vec3(0.0);
    for (int x = -1 ; x <= 1 ; x++)
    {
        for (int y = -1 ; y <= 1 ; y++)
        {
            //soDep = LinearizeDepth(texture( posTex, UV + vec2(windowScale.x * x, windowScale.y * y)).z);
			//tempCol = texture( colTex, UV + vec2(windowScale.x * x, windowScale.y * y)).rgb;
			tempCol = texture( colTex, UV + vec2(windowScale.x * x, windowScale.y * y)).rgb;
            sumX += conMatX[x+1][y+1] * tempCol;// * conMatX[x+1][y+1];
            sumY += conMatY[x+1][y+1] * tempCol;// * conMatY[x+1][y+1];
	    }
    }
   //float val = sqrt(sumX * sumX + sumY * sumY);
	//   vec3 sum = sqrt(sumX * sumX + sumY * sumY);
	   vec3 sum = sqrt(sumX.z * sumX.z + sumY.z * sumY.z);
   //float choice = float(val > 0.01);
   float choice = float(length(sum) > 0.98);
   choice = float(dot(normalize(sumY + sumX), norm) > 0.2);
   color.rgb = choice * vec3(0.1) + (1.0-choice) * color.rgb;
   //color.rgb = vec3(0.5 * soDep + 0.5);
#endif
	
	//float dd = getAmbient();
	//color.rgb = vec3(dd);
//	color.rgb = vec3(getAmbient());
//	color.rgb = vec3(0.5 - pos.xy/250, 0.5);
//	color.rgb = 0.5 + 0.5 * norm;

//	color.rgb = (vec3(0.5) + 0.5 * vec3(pos)) / 40.0;

	color.a = 1.0;
	//color.rgb = vec3(mappedLight * col.b);
	//color.rgb = vec3(UV, 0.5);
}