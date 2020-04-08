#pragma once
#include <Engine.hpp>
class ColorPalette
{
public:
	std::vector<GLfloat> hsList;
	void addHueSat(float hue, float sat);
};

