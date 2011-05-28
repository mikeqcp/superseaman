#include "Water.h"


Water::Water(string fileName, glm::mat4 P, glm::mat4 V, glm::mat4 M, string vshaderFile, string fshaderFile){

	this ->P = P;
	this ->V = V;
	this ->M = M;

	time = 1.0f;
	time2 = 1.0f;

	clipPlane = glm::vec4(0, 0, 0, 0);
	SetupShaders(vshaderFile, fshaderFile);
	Load(fileName, 0, 0);
	verticesBufferType = GL_STATIC_DRAW;
	SetupBuffers();

}


Water::~Water(void)
{
}

void Water::SetupUniformVariables(){

	//zmienne jednorodne dla calego modelu
	glUniform3f(locLookAtPos, lookAtPos.x, lookAtPos.y, lookAtPos.z);
	glUniform3f(locViewPos, viewPos.x, viewPos.y, viewPos.z);
	glUniform4f(locClipPlane, clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);

	glUniform1i(locTextureMap0, 0);
	glUniform1i(locTextureMap1, 1);
	glUniform1i(locTextureMap2, 2);
	glUniform1i(locTextureMap3, 3);
	glUniform1i(locTextureMap4, 4);

	glUniform1f(locTime, time);
	glUniform1f(locTime2, time2);

}

void Water::SetUpDrawing(){

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normalMapTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, dudvMapTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, depthTex);

}

void Water::SetupShaders(string vshaderFile, string fshaderFile){

	vertexShader = LoadShader(GL_VERTEX_SHADER, vshaderFile);
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fshaderFile);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}

	locP = glGetUniformLocation(shaderProgram, "P");
	locV = glGetUniformLocation(shaderProgram, "V");
	locM = glGetUniformLocation(shaderProgram, "M");
	locLighPos = glGetUniformLocation(shaderProgram, "lightPos");
	locViewPos = glGetUniformLocation(shaderProgram, "viewPos");
	locLookAtPos = glGetUniformLocation(shaderProgram, "lookAtPos");

	locTime = glGetUniformLocation(shaderProgram, "time");
	locTime2 = glGetUniformLocation(shaderProgram, "time2");

	locMaterial[0] = glGetUniformLocation(shaderProgram, "ambient");
	locMaterial[1] = glGetUniformLocation(shaderProgram, "diffuse");
	locMaterial[2] = glGetUniformLocation(shaderProgram, "specular");

	locTextureMap0 = glGetUniformLocation(shaderProgram, "textureMap0");
	locTextureMap1 = glGetUniformLocation(shaderProgram, "textureMap1");
	locTextureMap2 = glGetUniformLocation(shaderProgram, "textureMap2");
	locTextureMap3 = glGetUniformLocation(shaderProgram, "textureMap3");
	locTextureMap4 = glGetUniformLocation(shaderProgram, "textureMap4");

	locTexCoord = glGetAttribLocation(shaderProgram, "texCoord");

	locVertex = glGetAttribLocation(shaderProgram, "vertex");
	locNormal = glGetAttribLocation(shaderProgram, "normal");

}

void Water::Draw(){
	
	glUseProgram(shaderProgram);

	SetupUniformVariables();

	SetUpDrawing();

	for(unsigned i = 0; i < meshCount; i++){
	
		meshes[i].Draw();

	}

	time += 0.005f;
	time2 += 0.01f;

}