#pragma once
#include "model.h"
class Water :
	public Model
{
private:

	GLuint reflectionTex, refractionTex, depthTex;

	void SetupUniformVariables();

	void SetUpDrawing();

public:
	Water(string fileName, glm::mat4 P, glm::mat4 V, glm::mat4 M, string vshaderFile, string fshaderFile);
	~Water(void);

	void SetReflectionTex(GLuint reflectionTex){ this->reflectionTex = reflectionTex; };

	void Draw();

}; 

