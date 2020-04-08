#version 330 core

in vec2 UV;

out float ao;

uniform sampler2D posTex;
uniform sampler2D normTex;
uniform sampler2D texNoise;

#define KERNAL_SIZE 32

uniform vec3 samples[KERNAL_SIZE];//64
uniform mat4 projection;
uniform mat4 viewMatrix;

uniform vec2 noiseScale;
uniform vec2 windowScale;

const int kernelSize = 64;
const float radius = 0.5;//1.5 0.4
const float bias = 0.08; //0.06
float gSampleRad = 0.6;

const mat3 conMatX = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
};

const mat3 conMatY = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

const float [] conDubMatX = {
    1,  2, 0,  -2 , -1,
    4,  8, 0,  -8 , -4,
    6, 12, 0, -12 , -6,
    4,  8, 0,  -8 , -4,
    1,  2, 0,  -2 , -1
};

const float[] conDubMatY = {
    1,  2, 0,  -2, -1,
    4,  8, 0,  -8, -4,
    6, 12, 0, -12, -6,
    4,  8, 0,  -8, -4,
    1,  2, 0,  -2, -1
};



uniform float zNear = 0.1;
uniform float zFar = 1000.0;

float linearDepth(float depthSample)
{
    return depthSample;
    depthSample = 2.0 * depthSample - 1.0;
    float zLinear = 2.0 * zNear * zFar / (zFar + zNear - depthSample * (zFar - zNear));
    return zLinear;
}

float depthSample(float linearDepth)
{
    float nonLinearDepth = (zFar + zNear - 2.0 * zNear * zFar / linearDepth) / (zFar - zNear);
    nonLinearDepth = (nonLinearDepth + 1.0) / 2.0;
    return nonLinearDepth;
}

float signum(float x)
{
    return x >= 0 ? 1.0 : -1.0;
}

//void main()
//{
//   float occlusion = 0.0;
//
//	vec3 fragPos   = texture(posTex, UV).xyz;
//	vec3 normal    = texture(normTex, UV).xyz;
//	vec3 randomVec = texture(texNoise, UV * noiseScale).xyz;  
//
//
//    vec3 nx = dFdx(normal);
//    vec3 ny = dFdy(normal);
//
//    float zx = dFdx(normal.z);
//    float zy = dFdy(normal.z);
//
//    mat4 PVMat = projection * viewMatrix;
//
//    mat4 invMat = inverse(viewMatrix);
//
//   // float sumX = 0.0;
//	//float sumY = 0.0;
//   // float tempCol = 0.0;
//
//    vec3 sumPX = vec3(0.0);
//    vec3 sumPY = vec3(0.0);
//    vec3 tempP = vec3(0.0);
//
//    vec3 sumNX = vec3(0.0);
//    vec3 sumNY = vec3(0.0);
//    vec3 tempN = vec3(0.0);
//
//    vec3 avgPos = vec3(0);
//    vec3 avgNorm = vec3(0);
//
//    //float scale = 2 /(PVMat * vec4(fragPos,1)).z;//10 / fragPos.z;
//
//    flaot scale = radius / fragPos.z;
//
//    for (int x = -1 ; x <= 1 ; x++)
//    {
//        for (int y = -1 ; y <= 1 ; y++)
//        {
//			tempP = texture( posTex, UV + scale * vec2(windowScale.x * x, windowScale.y * y)).xyz;
//          //  tempP =  (invMat * vec4(tempP,1)).xyz;
//            sumPX += conMatX[x+1][y+1] * tempP;
//            sumPY += conMatY[x+1][y+1] * tempP;
//
//
//           // vec3 scaledP = texture( posTex, UV + vec2(windowScale.x * x, windowScale.y * y)).xyz;
//            avgPos += tempP / 9.0;
//
//            tempN = texture( normTex, UV + scale * vec2(windowScale.x * x, windowScale.y * y)).xyz;
//            sumNX += conMatX[x+1][y+1] * tempN;
//            sumNY += conMatY[x+1][y+1] * tempN;
//
//            avgNorm += tempN / 9.0;
//	    }
//    }
//
//   //float val = sqrt(sumX * sumX + sumY * sumY);
//   //vec3 sum = (sumX.z + sumY.z > 0.0 ? 1.0 : -1.0) * sqrt(sumX * sumX + sumY * sumY);
//   //float choice = float(val > 0.01);
//   //float choice = float(length(sum) > 0.98);
//
//  // float sumZ = length(vec2(sumZX, sumZY));
//   //vec2 gradZ = vec2(sumZX, sumZY);
//   //vec2 grad = vec2(sumX.z, sumY.z);
//   //vec2 grad = vec2(sumNX.z, sumNY.z);
//  // float val = dot(grad, normal.xy);
//   
//   
// //  float normVal = length(grad);
//   //float zVal = length(gradZ);
//
//   //float val = (normVal >= 0.5 && zVal <= 0.8) ? 1.0 : 0.0; 
//
//
//   //sumPX.z = linearDepth(sumPX.z);
//   //sumPY.z = linearDepth(sumPY.z);
//
//
//   float lenPos = length(sumPX * sumPX + sumPY * sumPY);
//   float lenNorm = length(sumNX * sumNX + sumNY * sumNY);
//
//
//
//   float val = length(vec2(dot(-sumPX, sumNX), dot(-sumPY, sumNY)));         //lenPos;//(lenNorm > 0.8) ? 1.0 : 0.0;// dot(normal, sumNX);// //lenNorm;//lenPos;
//   
//
//   if(fragPos.x < 0)
//   {
//     //float sobel = length(sqrt(sumNX * sumNX + sumNY * sumNY));
//     //float posSobel = length(sqrt(sumPX * sumPX + sumPY * sumPY));
//     //ao = sobel * (1.0 - posSobel); //length((invMat * vec4(fragPos,1)).xyz) / 50.0;
//     float d = length(avgPos - fragPos);
//     d = length(avgNorm - normal);
//     ao = 0.5 * d;
//   }
//   else
//   {
//    //  ao = length(sqrt(sumNX * sumNX + sumNY * sumNY));
//      //ao = dot(normal, sumNX) + dot(normal, sumNY);
//      // ao = fragPos.z;
//
//        //ao = 1.0;
//   }
//
//    float d = length(avgPos - fragPos);
//     d = length(avgNorm - normal);
//     ao = 0.5 * d;
//
//   ao = 1 - ao;
//   
//   
//   //if(fragPos.z < 0)
//   // ao = 1.0;
//
// //  ao = ao < 0.1 ? 0.0 : 1.0;
//
//   //float val = length(grad);//dot(grad, gradZ);
//
//   //val = sumZ < 0.8 ? val : 0.0;
//
//   //val = val > 0.8 ? val : 0.0;
//   //vec2 dGrad = fwidth(grad);
//   //float dd = sumX.z * signum(fwidth(sumX.z));
//
//  // ao = 0.5 + 0.5 * val;//0.5 + 0.5 * val;//length(grad);//sum.z;
//}


//THIS ONE WORKS!!!
void main()
{
   float occlusion = 0.0;

	vec3 fragPos   = texture(posTex, UV).xyz;
	vec3 normal    = texture(normTex, UV).xyz;
	vec3 randomVec = texture(texNoise, UV * noiseScale).xyz;  

    float rad = radius / fragPos.z;

    for(int i = 0; i < KERNAL_SIZE; i++)
    {
       // vec2 coords = samples[i].xy;

         vec2 coords = reflect(samples[i].xy, randomVec.xy) * rad;

         vec3 diff = texture(posTex, coords + UV).xyz - fragPos;
         vec3 v = normalize(diff);
         float d = length(diff);
         
         occlusion += max(0.0, dot(normal, v) - bias)*(1.0 / (1.0 + d));
	}
    ao = 1.0 - (occlusion / KERNAL_SIZE);
}




//void main()
//{
//    //vec3 norm = texture(normTex, UV).xyz;
//   // vec3 pos = texture(posTex, UV).xyz;
//    vec3 sumX = vec3(0.0);
//	vec3 sumY = vec3(0.0);
//     vec3 tempCol = vec3(0.0);
//    for (int x = -1 ; x <= 1 ; x++)
//    {
//        for (int y = -1 ; y <= 1 ; y++)
//        {
//            //soDep = LinearizeDepth(texture( posTex, UV + vec2(windowScale.x * x, windowScale.y * y)).z);
//			//tempCol = texture( colTex, UV + vec2(windowScale.x * x, windowScale.y * y)).rgb;
//			tempCol = texture( normTex, UV + vec2(windowScale.x * x, windowScale.y * y)).xyz;//vec3(depthSample(texture( posTex, UV + vec2(windowScale.x * x, windowScale.y * y)).z));
//            sumX += conMatX[x+1][y+1] * tempCol;// * conMatX[x+1][y+1];
//            sumY += conMatY[x+1][y+1] * tempCol;// * conMatY[x+1][y+1];
//	    }
//    }
//   //float val = sqrt(sumX * sumX + sumY * sumY);
//   vec3 sum = (sumX.z + sumY.z > 0.0 ? 1.0 : -1.0) * sqrt(sumX * sumX + sumY * sumY);
//   //float choice = float(val > 0.01);
//   float choice = float(length(sum) > 0.98);
//   ao = 0.5 + 0.5 * sum.z;//length(sum);//0.5 + 0.5 *dot(normalize(sumY + sumX), norm);//float(dot(normalize(sumY + sumX), norm) > 0.2);
//  
//   //color.rgb = vec3(0.5 * soDep + 0.5);
//}

//void main()
//{
//	float occlusion = 0.0;
//
//	vec3 fragPos   = texture(posTex, UV).xyz;//(viewMatrix * vec4(texture(posTex, UV).xyz,1)).xyz;
//	vec3 normal    = texture(normTex, UV).xyz;//(viewMatrix * vec4(texture(normTex, UV).xyz, 0)).xyz;
//	vec3 randomVec = texture(texNoise, UV * noiseScale).xyz;  
//
//	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
//	vec3 bitangent = normalize(cross(normal, tangent));
//	mat3 TBN       = mat3(tangent, bitangent, normal);  
//	
//	for(int i = 0; i < KERNAL_SIZE; i++)//++i
//	{
//		// get sample position
//		vec3 samp = TBN * samples[i]; // From tangent to view-space
//		//vec3 samp = randomVec - samples[i] * dot(randomVec, samples[i]); //samples[i];
//		//vec3 samp = samples[i]; // From tangent to view-space
//		samp = fragPos + samp * radius; 
//
//		vec4 offset = vec4(samp, 1.0);
//		offset      = projection * offset;    // from view to clip-space
//		offset.xyz /= offset.w;               // perspective divide
//		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  
//
//		float sampleDepth = texture(posTex, offset.xy).z; //(viewMatrix * vec4(texture(posTex, offset.xy).xyz, 1.0)).z; 
//
//		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
//		float oobCheck = float(sampleDepth < 0);// ? 0.0 : 1.0;
//		float depth = float(sampleDepth >= samp.z + bias);
//		//float tooFar = float((abs(fragPos.z - sampleDepth) < gSampleRad));
//
////		vec3 normTP = texture(normTex, tpInCS.xy).xyz;
////		vec3 dP = depthPoint - cameraPoint.xyz;
////
////
//		//vec3 d = vec3(0,0,-1);
//		//float bias = (0.5 * sampleDepth + 0.5) * (1.0 - dot(normal, normalize(fragPos)));//
//		//float bias_ = max(0.14 * (1.0 - dot(normal, normalize(fragPos))), 0.02);
//
//		occlusion += ((sampleDepth >= samp.z + bias) ? 1.0 : 0.0) * rangeCheck;// * oobCheck;   
//		//occlusion += depth * oobCheck * smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));// depth * tooFar * rangeCheck;// * oobCheck;   
//	}  
//
//	occlusion = 1.0 - (occlusion / KERNAL_SIZE);
//	ao = occlusion;
//	ao = -fragPos.z/500.0;// / 500.0;
//	//vec3 d = vec3(0,0,-1);
//	//ao = dot(normal, d);
////2	ao = dot(normal, -normalize(fragPos));
//}



//float gSampleRad = 0.2;
//void main()
//{
//	vec3 fragPos   = texture(posTex, UV).xyz;//(viewMatrix * vec4(texture(posTex, UV).xyz,1)).xyz;
//	//vec3 normal    = texture(normTex, UV).xyz;//(viewMatrix * vec4(texture(normTex, UV).xyz, 0)).xyz;
//	//vec3 randomVec = texture(texNoise, UV * noiseScale).xyz;  
//
////	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
////	vec3 bitangent = cross(normal, tangent);
////	mat3 TBN       = mat3(tangent, bitangent, normal);  
//	
//	float occlusion = 0.0;
//	for(int i = 0; i < 32; ++i)
//	{
//	    // get sample position
//	   vec3 samp = fragPos + radius * samples[i]; // From tangent to view-space
//	   // vec3 samp = samples[i]; // From tangent to view-space
//
//		vec4 offset = vec4(samp, 1.0);
//		offset      = projection * offset;    // from view to clip-space
//		offset.xyz /= offset.w;               // perspective divide
//		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  
//
//		float sampleDepth = texture(posTex, offset.xy).z;
//		occlusion += (abs(fragPos.z - sampleDepth) < gSampleRad ? step(sampleDepth, samp.z) : 0.0);
//		//occlusion += float(sampleDepth < samp.z);// (abs(fragPos.z - sampleDepth) < radius ? step(sampleDepth, samp.z) : 0.0);
//	}  
//	
//	ao = 1.0 - (occlusion / 32.0);
//}

