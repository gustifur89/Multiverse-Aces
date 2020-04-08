#include "HSFShader.h"

// ================================= HSFShader ==============================

HSFShader::HSFShader()
{

}

std::shared_ptr<HSFShader> HSFShader::loadShader(std::string fileName)
{
	std::shared_ptr<HSFShader> out(new HSFShader());
	out->Shader::loadShader_(fileName, fileName);
	out->mvp = out->getUniformLocation("MVP");
	out->nm = out->getUniformLocation("NM");
	out->hl = out->getUniformLocation("hsList");
	out->mv = out->getUniformLocation("MV");
	return out;
}

void HSFShader::setMatrixes(glm::mat4 MVP, glm::mat4 NM, glm::mat4 MV, std::vector<GLfloat> hsList)
{
	loadMatrix(mvp, MVP);
	loadMatrix(nm, NM);
	loadMatrix(mv, MV);
	//loadArray(hl, hueList);
	glUniform1fv(hl, hsList.size(), &hsList[0]);
}

HSFShader::~HSFShader()
{
}
