#pragma once
#include "model.h"
class Water :
	public Model
{
private:

	GLuint reflectionTex, refractionTex, depthTex, normalMapTex, dudvMapTex;
	GLuint locTime, locTime2;
	GLuint locTextureMap1, locTextureMap2, locTextureMap3, locTextureMap4;
	GLfloat time, time2;	

	void SetupUniformVariables();

	void SetUpDrawing();

	void SetupLocations();

	void SetupShaders(string vshaderFile, string fshaderFile);

public:
	Water(string fileName, glm::mat4 P, glm::mat4 V, glm::mat4 M, string vshaderFile, string fshaderFile);
	~Water(void);

	void SetReflectionTex(GLuint reflectionTex){ this->reflectionTex = reflectionTex; };

	void SetRefractionAndDepthTex(GLuint refractionTex, GLuint depthTex){ this ->refractionTex = refractionTex; this->depthTex = depthTex; };

	void SetNormalAndDUDVMapsTex(GLuint normalMapTex, GLuint dudvMapTex){ this ->normalMapTex = normalMapTex; this->dudvMapTex = dudvMapTex; };

	void Draw();

	

}; 

