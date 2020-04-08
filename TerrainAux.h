#pragma once
#include <Engine.hpp>
#include "GameObjectHSF.h"
#include "PlayerExtension.h"
#include "ColorPalette.h"


struct BiomeData 
{
public:
	float hillDensity;
	float hillSharpness;
	float hillHeight;
	float hillScale;
	float offset;
	std::vector<float> roughnessData; // scale, weight
	std::vector<glm::vec4> heightColors;

	void addHeightColor(glm::vec4 hCol)
	{
		//need to sort insert it.
		heightColors.push_back(hCol);
		//sort the vector. An insertion
		for (int i = 0; i < heightColors.size(); i++)
		{
			int minIndex = i;
			float currentHeight = heightColors[i].x;
			for (int j = i + 1; j < heightColors.size(); j++)
			{
				if (heightColors[j].x < currentHeight)
				{
					minIndex = j;
					currentHeight = heightColors[j].x;
				}
			}
			//swap
			glm::vec4 temp = heightColors[i];
			heightColors[i] = heightColors[minIndex];
			heightColors[minIndex] = temp;
		}
	}

	glm::vec3 getColorData(float height)
	{
		for (int i = 0; i < heightColors.size(); i++)
		{
			if (height <= heightColors[i].x || i == heightColors.size() - 1)
			{
				//it must be larger than the previous but less than this...
				return heightColors[i].yzw();
			}
		}
		return glm::vec3(0, 0, 0);
	}

	void addRoughness(float scale, float weight)
	{
		roughnessData.push_back(1.0 / scale);
		roughnessData.push_back(weight);
	}
};


struct MapData
{
public:
	std::vector<BiomeData> biomes;
	std::vector<std::vector<int>> biomeMap;
};
