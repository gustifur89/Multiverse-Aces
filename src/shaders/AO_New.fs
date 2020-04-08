#version 330 core

in vec2 UV;

out float ao;

uniform sampler2D posTex;
uniform sampler2D normTex;
uniform sampler2D texNoise;

uniform vec3 samples[32];//64
uniform mat4 projection;
uniform mat4 viewMatrix;

uniform vec2 noiseScale;
uniform vec2 windowScale;

const int kernelSize = 16;
const float radius = 0.5;//1.5
const float bias = 0.2; //0.06


//void main()
//{
//	vec3 fragPos   = texture(posTex, UV).xyz;//(viewMatrix * vec4(texture(posTex, UV).xyz,1)).xyz;
//	vec3 normal    = texture(normTex, UV).xyz;//(viewMatrix * vec4(texture(normTex, UV).xyz, 0)).xyz;
//	vec3 randomVec = texture(texNoise, UV * noiseScale).xyz;  
//
//	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
//	vec3 bitangent = cross(normal, tangent);
//	mat3 TBN       = mat3(tangent, bitangent, normal);  
//	
//	float occlusion = 0.0;
//	for(int i = 0; i < 16; ++i)
//	{
//	    // get sample position
//	   vec3 samp = TBN * samples[i]; // From tangent to view-space
//	   // vec3 samp = samples[i]; // From tangent to view-space
//	    samp = fragPos + samp * radius; 
//
//		vec4 offset = vec4(samp, 1.0);
//		offset      = projection * offset;    // from view to clip-space
//		offset.xyz /= offset.w;               // perspective divide
//		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  
//
//		float sampleDepth = texture(posTex, offset.xy).z; //(viewMatrix * vec4(texture(posTex, offset.xy).xyz, 1.0)).z; 
//
//		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
//		occlusion += (sampleDepth >= samp.z + bias ? 1.0 : 0.0) * rangeCheck;   
//	}  
//	
//	occlusion = 1.0 - (occlusion / 16);
//	ao = occlusion;  
//
//}


void main()
{
	vec3 fragPos   = texture(posTex, UV).xyz;//(viewMatrix * vec4(texture(posTex, UV).xyz,1)).xyz;
	//vec3 normal    = texture(normTex, UV).xyz;//(viewMatrix * vec4(texture(normTex, UV).xyz, 0)).xyz;
	//vec3 randomVec = texture(texNoise, UV * noiseScale).xyz;  

//	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
//	vec3 bitangent = cross(normal, tangent);
//	mat3 TBN       = mat3(tangent, bitangent, normal);  
	
	float occlusion = 0.0;
	for(int i = 0; i < 32; ++i)
	{
	    // get sample position
	   vec3 samp = fragPos + radius * samples[i]; // From tangent to view-space
	   // vec3 samp = samples[i]; // From tangent to view-space

		vec4 offset = vec4(samp, 1.0);
		offset      = projection * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  

		float sampleDepth = texture(posTex, offset.xy).z;

		occlusion += (abs(fragPos.z - sampleDepth) < radius ? step(sampleDepth, samp.z) : 0.0);
	}  
	
	ao = 1.0 - (occlusion / 32.0);
}

