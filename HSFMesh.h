#pragma once
#include <Mesh.h>
#include "ColorPalette.h"

class HSFMesh : public Mesh
{
private:

public:
	~HSFMesh();
	GLuint svhBufferID;
	std::vector<GLfloat> viBuffer;
	std::vector<GLfloat> hsList;
	std::shared_ptr<ColorPalette> colorPalette;

	void bindArrays();
	static std::shared_ptr<HSFMesh> loadFromFile(std::string fileName);
};
