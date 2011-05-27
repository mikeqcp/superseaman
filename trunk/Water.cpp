#include "Water.h"


Water::Water(string fileName, glm::mat4 P, glm::mat4 V, glm::mat4 M, string vshaderFile, string fshaderFile) : Model(fileName, P, V, M, vshaderFile, fshaderFile)
{
}


Water::~Water(void)
{
}

void Water::SetupUniformVariables(){

	//zmienne jednorodne dla calego modelu
	glUniform3f(locLookAtPos, lookAtPos.x, lookAtPos.y, lookAtPos.z);
	glUniform4f(locClipPlane, clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
	glUniform1i(locTextureMap0, 0);

}

void Water::SetUpDrawing(){

	glBindTexture(GL_TEXTURE_2D, reflectionTex);

}

void Water::Draw(){
	
	glUseProgram(shaderProgram);

	SetupUniformVariables();

	SetUpDrawing();

	for(unsigned i = 0; i < meshCount; i++){
	
		meshes[i].Draw();

	}

}