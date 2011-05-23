#pragma once
#include <vector>

#include "includes.h"
#include "Material.h"
#include "ModelStuctures.h"
#include "Mesh.h"

using namespace std;

class Model
{
protected:

	glm::mat4 P, V, M;
	glm::vec4 lightPos;

	GLuint locP, locV, locM, locVertex, locNormal, locLighPos;
	GLuint locMaterial[3];
	GLuint vertexShader, fragmentShader, shaderProgram;
	GLuint bufVertices, bufIndices, bufNormals, vao;
	GLuint verticesBufferType;
	GLuint verticesCount, normalsCount, indicesCount;

	GLfloat *vertices, *normals;

	glm::vec4 *originalVertices;
	unsigned originalVerticesCount;

	GLuint *indices;

	Mesh *meshes;
	unsigned meshCount;

	Texture *textures;
	unsigned textureCount;

	Material *materials;
	int materialCount;

	GLfloat rotationAngle;
	glm::vec3 rotationAxis;

	GLuint LoadShader(GLenum shaderType, string fileName);
	void SetupBuffers();
	void CleanShaders();

	void SetupUniformVariables();

	void SetupShaders(string vshaderFile, string fshaderFile);
	void CleanBuffers();


public:
	Model();
	Model(string fileName, glm::mat4 P, glm::mat4 V, glm::mat4 M, string vshaderFile, string fshaderFile);
	~Model(void);

	

	void Update(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos);

	void UpdateMesh(string name, glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos);

	void Draw();

	void Load(string fileName, Texture *textures, int textureCount);

	void UpdateRotation(GLfloat angle, glm::vec3 axis);

	glm::mat4 GetModelMatrix();

	glm::vec4 *GetSegment(string mname, string msname, int *length);


};

