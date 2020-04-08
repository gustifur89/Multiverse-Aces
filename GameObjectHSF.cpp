#include "GameObjectHSF.h"

// ============================== GameObjectHSF

GameObjectHSF::GameObjectHSF()
{
	colorPalette = NULL;
}

GameObjectHSF::~GameObjectHSF()
{
}

void GameObjectHSF::setColorPalette(std::shared_ptr<ColorPalette> colorPalette)
{
	this->colorPalette = colorPalette;
}

std::shared_ptr<ColorPalette> GameObjectHSF::getColorPalette()
{
	return colorPalette;
}

void GameObjectHSF::renderFunc(Camera& camera, glm::mat4 parentTransform)
{
	glm::mat4 MVMatrix = camera.getTransform() * parentTransform * transform.getTransform();
	glm::mat4 MVPmatrix = camera.getProjection() * MVMatrix;
	glm::mat4 NMmatrix = glm::transpose(glm::inverse(MVMatrix));
	//glm::mat4 MVPmatrix = camera.getProjection() * camera.getTransform() * parentTransform * transform.getTransform();
	//glm::mat4 NMmatrix = camera.getTransform() * parentTransform * transform.getTransform();//glm::transpose(glm::inverse(camera.getTransform() * parentTransform * transform.getTransform()));
	

	double windingFlip = glm::sign(glm::determinant(NMmatrix));
	if(windingFlip == -1)
		glCullFace(GL_FRONT);
	else
		glCullFace(GL_BACK);

	if (shader && mesh && this->getColorPalette())
	{
		shader->useShader();
		shader->setMatrixes(MVPmatrix, NMmatrix, MVMatrix, this->getColorPalette()->hsList);
		mesh->render();
	}
}
