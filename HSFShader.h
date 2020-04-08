#pragma once
#include <Shader.h>

class HSFShader : public Shader
{
public:
	int mvp, nm, hl, mv;
	HSFShader();
	~HSFShader();

	static std::shared_ptr<HSFShader> loadShader(std::string fileName);

	void setMatrixes(glm::mat4 MVP, glm::mat4 NM, glm::mat4 MV, std::vector<GLfloat> hsList);
};

