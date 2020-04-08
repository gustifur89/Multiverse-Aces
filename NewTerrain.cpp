#include "NewTerrain.h"

// ================================= NewChunk ================

NewChunk::NewChunk()
{
	persistentVisible = true;
}

NewChunk::~NewChunk()
{

}

/*
void NewChunk::colorMesh()
{
}

glm::vec2 NewChunk::getColor(double height, double normY)
{
	float i = 0;
	float v = 0;
	if (height < WORLD_SCALE * 0.0)
	{
		//lava
		i = 0;
		v = 0.98;
	}
	else if (height < WORLD_SCALE * 4)
	{
		i = 1;
		v = 0.60;
	}
	else
	{
		i = 2;
		v = 0.80;
	}

	if (normY < 0.008)
	{
		i = 3;
		v = 0.4;
	}


	//i = 2;
	//v = 0.30;

	return glm::vec2(v, i);
}

void NewChunk::meshFromVertexGrid(std::vector<std::vector<double>> & grid)
{
	int width = grid.size() - 1;
	int depth = grid[0].size() - 1;
	
	std::shared_ptr<HSFMesh> chunkMesh = std::shared_ptr<HSFMesh>(new HSFMesh());

	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < depth; z++)
		{
			//x from 0-width -> -1, 1-scale;

			double y0 = grid[x][z];
			double y1 = grid[x + 1][z];
			double y2 = grid[x][z + 1];
			double y3 = grid[x + 1][z + 1];

			double x_0 = ((double)x / width);
			double x_1 = ((double)(x + 1) / width);
			double z_0 = ((double)z / depth);
			double z_1 = ((double)(z + 1) / depth);

			//
			int index = x * (depth) + z;

			//v0
			glm::vec3 v0 = glm::vec3(x_0, y0, z_0);
			//v1
			glm::vec3 v1 = glm::vec3(x_1, y1, z_0);
			//v2
			glm::vec3 v2 = glm::vec3(x_0, y2, z_1);
			//v3
			glm::vec3 v3 = glm::vec3(x_1, y3, z_1);


			//FACE A: V0, V2, V1
			glm::vec3 normalA = Geometry::calculateNormal(v0, v2, v1);
			double heightA = fmax(v0.y, fmax(v2.y, v1.y));
			glm::vec2 hvA = getColor(heightA, normalA.y);

			chunkMesh->vertexBuffer.push_back(v0.x);
			chunkMesh->vertexBuffer.push_back(v0.y);
			chunkMesh->vertexBuffer.push_back(v0.z);
			chunkMesh->normalBuffer.push_back(normalA.x);
			chunkMesh->normalBuffer.push_back(normalA.y);
			chunkMesh->normalBuffer.push_back(normalA.z);
			chunkMesh->viBuffer.push_back(hvA.x);
			chunkMesh->viBuffer.push_back(hvA.y);
			chunkMesh->indexBuffer.push_back(6 * index + 0);

			chunkMesh->vertexBuffer.push_back(v2.x);
			chunkMesh->vertexBuffer.push_back(v2.y);
			chunkMesh->vertexBuffer.push_back(v2.z);
			chunkMesh->normalBuffer.push_back(normalA.x);
			chunkMesh->normalBuffer.push_back(normalA.y);
			chunkMesh->normalBuffer.push_back(normalA.z);
			chunkMesh->viBuffer.push_back(hvA.x);
			chunkMesh->viBuffer.push_back(hvA.y);
			chunkMesh->indexBuffer.push_back(6 * index + 1);

			chunkMesh->vertexBuffer.push_back(v1.x);
			chunkMesh->vertexBuffer.push_back(v1.y);
			chunkMesh->vertexBuffer.push_back(v1.z);
			chunkMesh->normalBuffer.push_back(normalA.x);
			chunkMesh->normalBuffer.push_back(normalA.y);
			chunkMesh->normalBuffer.push_back(normalA.z);
			chunkMesh->viBuffer.push_back(hvA.x);
			chunkMesh->viBuffer.push_back(hvA.y);
			chunkMesh->indexBuffer.push_back(6 * index + 2);

			//FACE B: V2, V3, V1
			glm::vec3 normalB = Geometry::calculateNormal(v2, v3, v1);
			double heightB = fmax(v2.y, fmax(v3.y, v1.y));
			glm::vec2 hvB = getColor(heightB, normalB.y);

			chunkMesh->vertexBuffer.push_back(v2.x);
			chunkMesh->vertexBuffer.push_back(v2.y);
			chunkMesh->vertexBuffer.push_back(v2.z);
			chunkMesh->normalBuffer.push_back(normalB.x);
			chunkMesh->normalBuffer.push_back(normalB.y);
			chunkMesh->normalBuffer.push_back(normalB.z);
			chunkMesh->viBuffer.push_back(hvB.x);
			chunkMesh->viBuffer.push_back(hvB.y);
			chunkMesh->indexBuffer.push_back(6 * index + 3);

			chunkMesh->vertexBuffer.push_back(v3.x);
			chunkMesh->vertexBuffer.push_back(v3.y);
			chunkMesh->vertexBuffer.push_back(v3.z);
			chunkMesh->normalBuffer.push_back(normalB.x);
			chunkMesh->normalBuffer.push_back(normalB.y);
			chunkMesh->normalBuffer.push_back(normalB.z);
			chunkMesh->viBuffer.push_back(hvB.x);
			chunkMesh->viBuffer.push_back(hvB.y);
			chunkMesh->indexBuffer.push_back(6 * index + 4);

			chunkMesh->vertexBuffer.push_back(v1.x);
			chunkMesh->vertexBuffer.push_back(v1.y);
			chunkMesh->vertexBuffer.push_back(v1.z);
			chunkMesh->normalBuffer.push_back(normalB.x);
			chunkMesh->normalBuffer.push_back(normalB.y);
			chunkMesh->normalBuffer.push_back(normalB.z);
			chunkMesh->viBuffer.push_back(hvB.x);
			chunkMesh->viBuffer.push_back(hvB.y);
			chunkMesh->indexBuffer.push_back(6 * index + 5);
		}
	}

	mesh = chunkMesh;
	//mesh->bindArrays();
	mesh->recalculateBounds();
}

double NewChunk::getGround(std::pair<float, float> pos, siv::PerlinNoise& noise)
{
	//scale back to proper size and offset for artifact reduction
	pos.first /= WORLD_SCALE;
	pos.second /= WORLD_SCALE;
	pos.first += 1E5;
	pos.second += 1E5;

	//Need a nice simple way to do this...

	double biomeValue = noise.octaveNoise(pos.first, pos.second, 2);



	double largeFactor = 0.7;
	double noiseScaleLarge = 1.0 / 500.0;
	
	double mediumFactor = 0.25;
	double noiseScaleMedium = 1.0 / 100.0;

	double smallFactor = 0.05;
	double noiseScaleSmall = 1.0 / 10.0;


	double large = largeFactor * noise.octaveNoise(noiseScaleLarge * pos.first, noiseScaleLarge * pos.second, 6);
	//Large will be mountains...   Need to get cusps...
	//0-1
	large = fabs(large - 0.5) * 2.0;

	double medium = mediumFactor * noise.octaveNoise(noiseScaleMedium * pos.first, noiseScaleMedium * pos.second, 6);
	double small = smallFactor * noise.octaveNoise(noiseScaleSmall * pos.first, noiseScaleSmall * pos.second, 6);


	//double flat = noise.octaveNoise0_1(pos.first, pos.second, 4);

	//return large + medium + small;

	double mountainScale = 1.0 / 100.0;
	double edgeSharpness = 1.6;

	double mountainHieghtDensity = noise.octaveNoise0_1(mountainScale * pos.first, mountainScale * pos.second, 6);
	mountainHieghtDensity = glm::clamp((float)mountainHieghtDensity, 0.0f, 1.0f);
	double mountainDensity = powf(mountainHieghtDensity, edgeSharpness);
	
	double factor = 1.0 / 400.0;
	double height = noise.octaveNoise0_1(factor * pos.first, factor * pos.second, 6);

	double h0 = height;

	height = fabs(1.0 - fabs(height - 0.5) * 2.0);
	double spikiness = 8.0;
	 
	double h1 = height;

	height = mountainDensity * powf(height, spikiness);

	double h2 = height;

	double riverFactor = 1.0 / 1000.0;
	double river = noise.octaveNoise0_1(riverFactor * (pos.first + 2.3E6), riverFactor * (pos.second + 2.3E6), 6);
	river = fabs(1.0 - fabs(river - 0.5) * 2.0);
	double riverSharpness = 9.0 ;
	river = 1.0 * powf(river, riverSharpness);

	double finalHeight = height + 0.5 * (large + medium + small);// -0.3 * river + 0.3 * small;// (large + medium + small);

	finalHeight *= (1.0 - river);

	finalHeight += -0.5 * river;

	if (finalHeight < 0.0)
		finalHeight /= 12.0;

	finalHeight -= 0.2 * small;

	return finalHeight;
}

void NewChunk::generateMesh(siv::PerlinNoise& noise, int segments, float maxHeight, float noiseScale)
{
	//mesh is from -1 to 1 on x and z.
	//this->colorMesh = colorPalette;

	double scale = 1.0 / segments;
	
	glm::vec2 posOffset = this->transform.position.xz() + glm::vec2(1.0 / 3.0);

	glm::vec2 size = this->transform.scale.xz();

	std::vector<std::vector<double>> grid(segments + 1, std::vector<double>(segments + 1, 0.0));

	for (int x = 0; x <= segments; x++)
	{
		for (int z = 0; z <= segments; z++)
		{
			double x_ = (double) x / segments;
			double z_ = (double) z / segments;
			glm::vec2 vPos = (posOffset + size * glm::vec2(x_, z_));
			std::pair<float, float> cPos = std::pair<float, float>(vPos.x, vPos.y);
			//cPos should be the position in terrain space. 
			double height = WORLD_SCALE * maxHeight * getGround(cPos, noise);
			grid[x][z] = height;
		}
	}

	meshFromVertexGrid(grid);		
}

*/
// ================================= NewTerrain =================

NewTerrain::NewTerrain()
{
	float chunkSize = 100.0;
	float vertexDensity = 0.25 / WORLD_SCALE;// 0.25 / WORLD_SCALE;
	
	size = glm::vec2(chunkSize, chunkSize);

	chunkupdateMutex = false;

	numSegments = chunkSize * vertexDensity;
	halfSize = 0.5f * size;
	chunkDistance = 2;
	chunkRemoveDistance = chunkDistance + 1;
	persistentVisible = true;
	groundNoise.reseed(100);
	temperatureNoise.reseed(101);
	humidityNoise.reseed(102);
	noiseScale = 1.0 / 30.0;
	maxHeight = 60.0;

	//assemble map data before we make an auto reader.
	/*std::vector<std::vector<int> > biomeMap{ 
		{ 0, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 2, 2, 1, 1 },
		{ 2, 2, 2, 1 } };
	*/
	std::vector<std::vector<int> > biomeMap{
		{ 0, 1, 2, 2 },
		{ 1, 1, 0, 2 },
		{ 2, 1, 1, 2 },
		{ 3, 1, 0, 0 } };
	mapData.biomeMap = biomeMap;


	//pink biome
	BiomeData bPink;
	bPink.offset = 0.0;
	bPink.heightColors.push_back(glm::vec4(0.5, 0.8, 0.66, 0.70)); //light pink
	bPink.heightColors.push_back(glm::vec4(0.9, 0.6, 0.45, 0.53)); //dark pink
	bPink.hillHeight = 0.0;// 0.4;
	bPink.hillDensity = 1.0 / 300.0;
	bPink.hillSharpness = 2.0;
	bPink.hillScale = 1.0 / 100.0;
	bPink.addRoughness(200, 0.8);
	bPink.addRoughness(20, 0.05);

	//slime biome
	BiomeData bSlime;
	bSlime.offset = 0.0;
	bSlime.addHeightColor(glm::vec4(0.2, 0.54, 0.80, 0.65)); //slime 
	bSlime.addHeightColor(glm::vec4(0.36, 0.54, 0.50, 0.4)); //sand
	bSlime.addHeightColor(glm::vec4(0.5, 0.66, 0.69, 0.66)); //rock
	bSlime.addHeightColor(glm::vec4(0.8, 0.54, 0.80, 0.65)); //slime 
	bSlime.hillHeight = 0.4;
	bSlime.hillDensity = 1.0 / 300.0;
	bSlime.hillSharpness = 5.0;
	bSlime.hillScale = 1.0 / 600.0;
	bSlime.addRoughness(100, 0.2);
	bSlime.addRoughness(20, 0.05);

	//ice biome
	BiomeData bIce;
	bIce.offset = 0.0;
	bIce.heightColors.push_back(glm::vec4(0.3, 0.41, 0.4, 0.8));
	bIce.heightColors.push_back(glm::vec4(1.0, 0.76, 0.76, 0.9));
	bIce.hillHeight = 1.0;
	bIce.hillDensity = 1.0 / 400.0;
	bIce.hillSharpness = 8.0;
	bIce.hillScale = 1.0 / 1000.0;
	bIce.addRoughness(300, 0.2);
	bIce.addRoughness(20, 0.05);

	//fire biome
	BiomeData bFire;
	bFire.offset = 0.0;
	bFire.heightColors.push_back(glm::vec4(0.2, 0.45, 0.16, 0.16)); // white plains - dark red
	bFire.heightColors.push_back(glm::vec4(0.9, 0.5, 0.2, 0.2)); // red hills
	bFire.hillHeight = 0.6;
	bFire.hillDensity = 1.0 / 200.0;
	bFire.hillSharpness = 10.0;
	bFire.hillScale = 1.0 / 200.0;
	bFire.addRoughness(100, 0.2);
	bFire.addRoughness(20, 0.05);
	

	mapData.biomes.push_back(bPink);
	mapData.biomes.push_back(bSlime);
	mapData.biomes.push_back(bIce);
	mapData.biomes.push_back(bFire);


	/*
	//biome 0 - dessert
	BiomeData biome0;
	biome0.offset = 0.0;
	biome0.heightColors.push_back(glm::vec4(0.0, 0.54, 0.50, 0.4));
	biome0.hillHeight = 0.5;
	biome0.hillSharpness = 12.0;
	biome0.hillScale = 1.0 / 300.0;
	//biome0.addRoughness(300, 0.15);
	biome0.addRoughness(25, 0.15);

	//biome 1 - plains
	BiomeData biome1;
	biome1.offset = 0.5;
	biome1.heightColors.push_back(glm::vec4(0.0, 0.29, 0.6, 0.34));
	biome1.hillHeight = 0.4;
	biome1.hillSharpness = 2.0;
	biome1.hillScale = 1.0 / 400.0;
	//biome1.addRoughness(600, 0.2);
	biome1.addRoughness(25, 0.15);

	//biome 2 - cold
	BiomeData biome2;
	biome2.offset = 1.0;
	biome2.heightColors.push_back(glm::vec4(0.0, 0.8, 0.8, 1.0));
	biome2.hillHeight = 1.0;
	biome2.hillSharpness = 5.0;
	biome2.hillScale = 1.0 / 400.0;
	//biome2.addRoughness(500, 0.2);
	biome2.addRoughness(25, 0.15);

	mapData.biomes.push_back(biome0);
	mapData.biomes.push_back(biome1);
	mapData.biomes.push_back(biome2);
	*/
}

NewTerrain::~NewTerrain()
{

}

void NewTerrain::setTargetEntity(std::shared_ptr<NewEntity> targetEntity)
{
	this->targetEntity = targetEntity;
}

void NewTerrain::startTerrainThread()
{
	
}

void NewTerrain::addChunks(glm::vec2 targetMapVertex)
{
	chunkupdateMutex = true;
	for (int x = -chunkDistance; x <= chunkDistance; x++)
	{
		for (int z = -chunkDistance; z <= chunkDistance; z++)
		{
			glm::vec2 mapVertex = targetMapVertex + glm::vec2(x, z);
			std::pair<float, float> point = std::pair<float, float>(mapVertex.x, mapVertex.y);

			//check if it is already made:
			if (chunkMap.find(point) == chunkMap.end())
			{
				//add a new one
				chunkMap[point] = generateChunk(mapVertex);
			}
		}
	}
	chunkupdateMutex = false;
}

void NewTerrain::bindChunkMeshes()
{
	//This is needed because the chunks are generated in a different thread, but they need to be added in the opengl thread.
	while (chunkMeshesToBind.size() > 0)
	{
		std::shared_ptr<NewChunk> chunkTemp = chunkMeshesToBind.top();
		chunkTemp->mesh->setUpVAO();
		chunkTemp->mesh->bindArrays();
		chunkMeshesToBind.pop();
	}


}

glm::vec3 NewTerrain::getBiome(std::pair<float, float> pos)
{
	double biomeScale = 800.0;//8000.0
	double invBiomeScale = 1.0 / biomeScale;

	double minTransitionRegion = 0.06;
	double biomeSmooth = 5.0;

	double biomeTemperature = clip(temperatureNoise.octaveNoise0_1(invBiomeScale * pos.first, invBiomeScale * pos.second, 2), 0.0, 0.999999);
	double biomeHumidity = clip(humidityNoise.octaveNoise0_1(invBiomeScale * pos.first, invBiomeScale * pos.second, 2), 0.0, 0.999999);
	
	std::vector<std::vector<int>> biomeMap = mapData.biomeMap;
	int size = biomeMap.size();


	float temp = biomeTemperature * size;
	float humid = biomeHumidity * size;

	int select0 = (int) floor(temp);
	int mainBiome = biomeMap[select0][0];

	int nearest = -1;
	float strength = 0.0;

	if (temp < select0 + 0.5)
	{
		nearest = clip(select0 - 1, 0, size-1);
		strength = fabs(select0 - temp); // this is the length to the edge, but this isn't in units I can use...
	}
	else
	{
		nearest = clip(select0 + 1, 0, size - 1);
		strength = fabs(select0 + 1 - temp);
	}

	int otherBiome = biomeMap[nearest][0];

	//strength = strength * biomeScale / biomeSmooth;

	if (strength > minTransitionRegion)
	{
		//nothing
		strength = 1.0;

	}
	else
	{
	//	if (otherBiome == mainBiome) strength = 0.5;
		//strength += 0.5;

		//strength = 0.5 * powf((1.0 / minTransitionRegion) * strength, biomeSmooth) + 0.5;// 0.5 * powf(strength, biomeSmooth);
		//strength = 
		strength = (strength / minTransitionRegion) * 0.5 + 0.5;

		//strength = 1.0;
	}

	if (false)
	{
		if (pos.first < 0)
		{
			if (pos.second < 0)
			{
				mainBiome = 0;
				if (fabs(pos.first) < fabs(pos.second))
				{
					otherBiome = 2;
				}
				else
				{
					otherBiome = 1;
				}
			}
			else
			{
				mainBiome = 1;
				if (fabs(pos.first) < fabs(pos.second))
				{
					otherBiome = 3;
				}
				else
				{
					otherBiome = 0;
				}
			}
		}
		else
		{
			if (pos.second < 0)
			{
				mainBiome = 2;
				if (fabs(pos.first) < fabs(pos.second))
				{
					otherBiome = 0;
				}
				else
				{
					otherBiome = 3;
				}
			}
			else
			{
				mainBiome = 3;
				if (fabs(pos.first) < fabs(pos.second))
				{
					otherBiome = 1;
				}
				else
				{
					otherBiome = 2;
				}
			}
		}
		strength = (1 / 100.0) * fmin(fabs(pos.first), fabs(pos.second)) + 0.5;
		strength = clip(strength, 0, 1);
	}
	
	return glm::vec3(mainBiome, otherBiome, strength);

	/*
	int select0 = (int) floor(temp);
	int select1 = (int) floor(humid);
	int mainBiome = biomeMap[select0][select1];

	//get the closest neighbor cell.


	int closest0 = glm::fract(temp) > 0.5 ? select0 + 1 : select0 - 1;
	int closest1 = glm::fract(humid) > 0.5 ? select1 + 1 : select1 - 1;

	closest0 = clip(closest0, 0, size - 1);
	closest1 = clip(closest1, 0, size - 1);
	int otherBiome = biomeMap[closest0][closest1];

	//now get how close to edge we are.


	//int otherBiome = biomeMap[clip(select0+1, 0, size-1)][clip(select1 + 1, 0, size-1)];

	float strength = glm::length(glm::vec2(temp, humid) - glm::vec2(select0, select1)) / size;

	return glm::vec3(mainBiome, otherBiome, strength);
	*/
}

float NewTerrain::getBiomeHeight(std::pair<float, float> pos, int biomeIndex)
{

	double hillDensity = groundNoise.octaveNoise0_1(mapData.biomes[biomeIndex].hillDensity * pos.first, mapData.biomes[biomeIndex].hillDensity * pos.second, 4);
	double hillValue = hillDensity * groundNoise.octaveNoise0_1(mapData.biomes[biomeIndex].hillScale * pos.first, mapData.biomes[biomeIndex].hillScale * pos.second, 4);
	hillValue = 1.0 - fabs(hillValue - 0.5);
	double hillHeight = mapData.biomes[biomeIndex].hillHeight * powf(hillValue, mapData.biomes[biomeIndex].hillSharpness);
	
	for (int i = 0; i < mapData.biomes[biomeIndex].roughnessData.size(); i += 2)
	{
 		double rough = groundNoise.octaveNoise0_1(mapData.biomes[biomeIndex].roughnessData[i] * pos.first, mapData.biomes[biomeIndex].roughnessData[i] * pos.second, 6);
		hillHeight += mapData.biomes[biomeIndex].roughnessData[i+1] * rough;
	}
	
	return hillHeight + mapData.biomes[biomeIndex].offset;
}

float NewTerrain::getHeight(std::pair<float, float> pos, int biomeIndex, int biomeIndex2, float strength)
{
	double h1 = getBiomeHeight(pos, biomeIndex);
	double h2 = getBiomeHeight(pos, biomeIndex2);

	return (strength) * h1 + (1.0 - strength) * h2;
}

glm::vec4 NewTerrain::getGround(std::pair<float, float> pos)
{
	//scale back to proper size and offset for artifact reduction
	pos.first /= WORLD_SCALE;
	pos.second /= WORLD_SCALE;
	//pos.first += 1E5;
	//pos.second += 1E5;
	
	glm::vec3 biomeData = getBiome(pos);

	float height = getHeight(pos, biomeData.x, biomeData.y, biomeData.z);

	glm::vec3 colorMain = mapData.biomes[biomeData.x].getColorData(height);
	glm::vec3 colorOther = mapData.biomes[biomeData.y].getColorData(height);
	glm::vec3 color = (biomeData.z) * colorMain + (1.0f - biomeData.z) * colorOther;

	return glm::vec4(height, color.r, color.g, color.b);
	/*
	double largeFactor = 0.7;
	double noiseScaleLarge = 1.0 / 500.0;

	double mediumFactor = 0.25;
	double noiseScaleMedium = 1.0 / 100.0;

	double smallFactor = 0.05;
	double noiseScaleSmall = 1.0 / 10.0;


	double large = largeFactor * noise.octaveNoise(noiseScaleLarge * pos.first, noiseScaleLarge * pos.second, 6);
	//Large will be mountains...   Need to get cusps...
	//0-1
	large = fabs(large - 0.5) * 2.0;

	double medium = mediumFactor * noise.octaveNoise(noiseScaleMedium * pos.first, noiseScaleMedium * pos.second, 6);
	double small = smallFactor * noise.octaveNoise(noiseScaleSmall * pos.first, noiseScaleSmall * pos.second, 6);


	//double flat = noise.octaveNoise0_1(pos.first, pos.second, 4);

	//return large + medium + small;

	double mountainScale = 1.0 / 100.0;
	double edgeSharpness = 1.6;

	double mountainHieghtDensity = noise.octaveNoise0_1(mountainScale * pos.first, mountainScale * pos.second, 6);
	mountainHieghtDensity = glm::clamp((float)mountainHieghtDensity, 0.0f, 1.0f);
	double mountainDensity = powf(mountainHieghtDensity, edgeSharpness);

	double factor = 1.0 / 400.0;
	double height = noise.octaveNoise0_1(factor * pos.first, factor * pos.second, 6);

	double h0 = height;

	height = fabs(1.0 - fabs(height - 0.5) * 2.0);
	double spikiness = 8.0;

	double h1 = height;

	height = mountainDensity * powf(height, spikiness);

	double h2 = height;

	double riverFactor = 1.0 / 2000.0;
	double river = 1.0 - noise.octaveNoise0_1(riverFactor * (pos.first), riverFactor * (pos.second), 6);
	river = fabs(1.0 - fabs(river - 0.5) * 2.0);
	double riverSharpness = 9.0;
	river = 1.0 * powf(river, riverSharpness);

	double finalHeight = height + 0.5 * (large + medium + small) - 2.0 * river;// -0.3 * river + 0.3 * small;// (large + medium + small);

	//finalHeight *= (1.0 - river);

	//finalHeight += -0.5 * river;

	if (finalHeight < 0.0)
		finalHeight /= 12.0;

	finalHeight -= 0.2 * small;

	return finalHeight;
	*/
}

std::shared_ptr<ColorMesh> NewTerrain::meshFromVertexGrid(std::vector<std::vector<glm::vec4>>& grid)
{
	int width = grid.size() - 1;
	int depth = grid[0].size() - 1;

	std::shared_ptr<ColorMesh> chunkMesh = std::shared_ptr<ColorMesh>(new ColorMesh());

	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < depth; z++)
		{
			//x from 0-width -> -1, 1-scale;

			glm::vec4 d0 = grid[x][z];
			glm::vec4 d1 = grid[x + 1][z];
			glm::vec4 d2 = grid[x][z + 1];
			glm::vec4 d3 = grid[x + 1][z + 1];


			double y0 = d0.x;
			double y1 = d1.x;
			double y2 = d2.x;
			double y3 = d3.x;

			double x_0 = ((double)x / width);
			double x_1 = ((double)(x + 1) / width);
			double z_0 = ((double)z / depth);
			double z_1 = ((double)(z + 1) / depth);

			int index = x * (depth)+z;

			//v0
			glm::vec3 v0 = glm::vec3(x_0, y0, z_0);
			//v1
			glm::vec3 v1 = glm::vec3(x_1, y1, z_0);
			//v2
			glm::vec3 v2 = glm::vec3(x_0, y2, z_1);
			//v3
			glm::vec3 v3 = glm::vec3(x_1, y3, z_1);


			//FACE A: V0, V2, V1
			glm::vec3 normalA = Geometry::calculateNormal(v0, v2, v1);

			chunkMesh->vertexBuffer.push_back(v0.x);
			chunkMesh->vertexBuffer.push_back(v0.y);
			chunkMesh->vertexBuffer.push_back(v0.z);
			chunkMesh->normalBuffer.push_back(normalA.x);
			chunkMesh->normalBuffer.push_back(normalA.y);
			chunkMesh->normalBuffer.push_back(normalA.z);
			//chunkMesh->viBuffer.push_back(d0.y);
			chunkMesh->colorBuffer.push_back(d0.y);
			chunkMesh->colorBuffer.push_back(d0.z);
			chunkMesh->colorBuffer.push_back(d0.w);
			chunkMesh->indexBuffer.push_back(6 * index + 0);

			chunkMesh->vertexBuffer.push_back(v2.x);
			chunkMesh->vertexBuffer.push_back(v2.y);
			chunkMesh->vertexBuffer.push_back(v2.z);
			chunkMesh->normalBuffer.push_back(normalA.x);
			chunkMesh->normalBuffer.push_back(normalA.y);
			chunkMesh->normalBuffer.push_back(normalA.z);
			chunkMesh->colorBuffer.push_back(d2.y);
			chunkMesh->colorBuffer.push_back(d2.z);
			chunkMesh->colorBuffer.push_back(d2.w);
			chunkMesh->indexBuffer.push_back(6 * index + 1);

			chunkMesh->vertexBuffer.push_back(v1.x);
			chunkMesh->vertexBuffer.push_back(v1.y);
			chunkMesh->vertexBuffer.push_back(v1.z);
			chunkMesh->normalBuffer.push_back(normalA.x);
			chunkMesh->normalBuffer.push_back(normalA.y);
			chunkMesh->normalBuffer.push_back(normalA.z);
			chunkMesh->colorBuffer.push_back(d1.y);
			chunkMesh->colorBuffer.push_back(d1.z);
			chunkMesh->colorBuffer.push_back(d1.w);
			chunkMesh->indexBuffer.push_back(6 * index + 2);

			//FACE B: V2, V3, V1
			glm::vec3 normalB = Geometry::calculateNormal(v2, v3, v1);

			chunkMesh->vertexBuffer.push_back(v2.x);
			chunkMesh->vertexBuffer.push_back(v2.y);
			chunkMesh->vertexBuffer.push_back(v2.z);
			chunkMesh->normalBuffer.push_back(normalB.x);
			chunkMesh->normalBuffer.push_back(normalB.y);
			chunkMesh->normalBuffer.push_back(normalB.z);
			chunkMesh->colorBuffer.push_back(d2.y);
			chunkMesh->colorBuffer.push_back(d2.z);
			chunkMesh->colorBuffer.push_back(d2.w);
			chunkMesh->indexBuffer.push_back(6 * index + 3);

			chunkMesh->vertexBuffer.push_back(v3.x);
			chunkMesh->vertexBuffer.push_back(v3.y);
			chunkMesh->vertexBuffer.push_back(v3.z);
			chunkMesh->normalBuffer.push_back(normalB.x);
			chunkMesh->normalBuffer.push_back(normalB.y);
			chunkMesh->normalBuffer.push_back(normalB.z);
			chunkMesh->colorBuffer.push_back(d3.y);
			chunkMesh->colorBuffer.push_back(d3.z);
			chunkMesh->colorBuffer.push_back(d3.w);
			chunkMesh->indexBuffer.push_back(6 * index + 4);

			chunkMesh->vertexBuffer.push_back(v1.x);
			chunkMesh->vertexBuffer.push_back(v1.y);
			chunkMesh->vertexBuffer.push_back(v1.z);
			chunkMesh->normalBuffer.push_back(normalB.x);
			chunkMesh->normalBuffer.push_back(normalB.y);
			chunkMesh->normalBuffer.push_back(normalB.z);
			chunkMesh->colorBuffer.push_back(d1.y);
			chunkMesh->colorBuffer.push_back(d1.z);
			chunkMesh->colorBuffer.push_back(d1.w);
			chunkMesh->indexBuffer.push_back(6 * index + 5);
		}
	}

	return chunkMesh;
}

void NewTerrain::generateMesh(std::shared_ptr<NewChunk> chunk)
{
	double scale = 1.0 / numSegments;

	glm::vec2 posOffset = chunk->transform.position.xz() + glm::vec2(1.0 / 3.0);

	glm::vec2 size = chunk->transform.scale.xz();

	std::vector<std::vector<glm::vec4>> grid(numSegments + 1, std::vector<glm::vec4>(numSegments + 1, glm::vec4(0.0)));
	
	for (int x = 0; x <= numSegments; x++)
	{
		for (int z = 0; z <= numSegments; z++)
		{
			double x_ = (double) x / numSegments;
			double z_ = (double) z / numSegments;
			glm::vec2 vPos = (posOffset + size * glm::vec2(x_, z_));
		//	std::cout << vPos.x << " : " << vPos.y << "\n";
			std::pair<float, float> cPos = std::pair<float, float>(vPos.x, vPos.y);
			//cPos should be the position in terrain space. 
			glm::vec4 ground = getGround(cPos);
			ground.x *= WORLD_SCALE * maxHeight;
			grid[x][z] = ground;
		}
	}

	chunk->mesh = meshFromVertexGrid(grid);
}

std::shared_ptr<NewChunk> NewTerrain::generateChunk(glm::vec2 mapVertex)
{
	std::shared_ptr<NewChunk> nChunk(new NewChunk());

	glm::vec2 posInTerrain = size * mapVertex;

	nChunk->transform.position = glm::vec3(posInTerrain.x, 0, posInTerrain.y);
	nChunk->transform.scale = glm::vec3(size.x, 1, size.y);

	//nChunk->generateMesh(noise, numSegments, maxHeight, noiseScale);

	generateMesh(nChunk);
	nChunk->mesh->recalculateBounds();
	nChunk->shader = this->shader;
//	nChunk->setColorPalette(this->colorPalette);
	chunkMeshesToBind.push(nChunk);
	return nChunk;
}

void NewTerrain::update()
{
	//add/remove chunks
	//remove chunks:

	glm::vec2 targetPosInTerrain = (glm::inverse(transform.getTransform()) * glm::vec4(targetEntity->transform.position, 1)).xz;
	//convert position to map position. This should get the closest vertex in the map.
	glm::vec2 targetMapVertex = glm::floor(targetPosInTerrain / size);
	
	for (auto p = chunkMap.cbegin(); p != chunkMap.cend();)
	{
		//check if farther away than chunkDistance
		glm::vec2 chunkVertex = glm::vec2(p->first.first, p->first.second);
		if (abs(chunkVertex.x - targetMapVertex.x) > chunkRemoveDistance || abs(chunkVertex.y - targetMapVertex.y) > chunkRemoveDistance)
		{
			chunkMap.erase(p++);
		}
		else
		{
			p++;
		}
	}

	if (USE_THREADING)
	{
		// Add new meshes
		if (chunkupdateMutex == false)
		{
			chunkupdateMutex = true;
			bindChunkMeshes();
			chunkupdateMutex = false;
		}

		//add chunks:
		if (chunkupdateMutex == false)
		{
			std::thread chunkManagement(&NewTerrain::addChunks, this, targetMapVertex);
			chunkManagement.detach();
		}
	}
	else
	{
		bindChunkMeshes();
		addChunks(targetMapVertex);
	}
	
}

void NewTerrain::renderFunc(Camera& camera, glm::mat4 parentTransform)
{
	for (auto const& x : chunkMap)
	{
		x.second->renderFunc(camera, parentTransform * this->transform.getTransform());
	}
}
