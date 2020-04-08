#pragma once
#include <GameObject.h>
#include "HSFShader.h"
#include "HSFMesh.h"
#include "ColorPalette.h"

class GameObjectHSF : public GameObject
{
protected:
	std::shared_ptr<ColorPalette> colorPalette;
public:
	GameObjectHSF();
	~GameObjectHSF();
	//std::vector<GLfloat> hueList;

	void setColorPalette(std::shared_ptr<ColorPalette> colorPalette);
	std::shared_ptr<ColorPalette> getColorPalette();

	void renderFunc(Camera& camera, glm::mat4 parentTransform);
//	void renderShadowFunc(std::shared_ptr<Shader> shader, int location);
	std::shared_ptr<HSFShader> shader;
};

