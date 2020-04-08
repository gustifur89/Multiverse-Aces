#pragma once
#include <Engine.hpp>
#include "GameObjectHSF.h"
#include "TerrainAux.h"
#include "PlayerExtension.h"

#define USE_THREADING true
#define WORLD_SCALE 0.2

class NewChunk : public GameObjectColor
{
public:
	std::pair<float, float> key;

	NewChunk();
	~NewChunk();

	/*
	void colorMesh();
	*/
	/*
	glm::vec2 getColor(double height, double normY);

	void meshFromVertexGrid(std::vector<std::vector<double>> & grid);

	double getGround(std::pair<float, float> pos, siv::PerlinNoise& noise);

	void generateMesh(siv::PerlinNoise &noise, int segments, float maxHeight, float noiseScale);
	*/
	//void renderFunc(Camera& camera, glm::mat4 parentTransform);
};

class NewTerrain : public GameObjectColor
{
protected:
	void addChunks(glm::vec2 targetMapVertex);
	
	bool chunkupdateMutex;

	std::stack<std::shared_ptr<NewChunk>> chunkMeshesToBind;

	void bindChunkMeshes();

	float clip(float n, float lower, float upper) {
		return std::max(lower, std::min(n, upper));
	}

public:
	//std::map<std::pair<double, double>, std::shared_ptr<newChunk>> chunkMap;
	std::map<std::pair<float, float>, std::shared_ptr<NewChunk>> chunkMap;
	std::shared_ptr<NewEntity> targetEntity;
	siv::PerlinNoise groundNoise;
	siv::PerlinNoise temperatureNoise;
	siv::PerlinNoise humidityNoise;
	MapData mapData;

	//important variables:
	glm::vec2 size;
	glm::vec2 halfSize;
	int chunkDistance;
	int chunkRemoveDistance;
	int numSegments;
	float noiseScale;
	float maxHeight;


	glm::vec3 getBiome(std::pair<float, float> pos);
	glm::vec3 getColor(double height, int biomeIndex);
	float getBiomeHeight(std::pair<float, float> pos, int biomeIndex);
	float getHeight(std::pair<float, float> pos, int biomeIndex, int biomeIndex2, float strength);
	glm::vec4 getGround(std::pair<float, float> pos);
	std::shared_ptr<ColorMesh> meshFromVertexGrid(std::vector<std::vector<glm::vec4>>& grid);
	void generateMesh(std::shared_ptr<NewChunk> chunk);
	std::shared_ptr<NewChunk> generateChunk(glm::vec2 mapVertex);

	NewTerrain();
	~NewTerrain();

	void setTargetEntity(std::shared_ptr<NewEntity> targetEntity);
	void startTerrainThread();

	void update();
	
	void renderFunc(Camera& camera, glm::mat4 parentTransform);
};

