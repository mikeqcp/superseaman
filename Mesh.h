#pragma once

#include <vector>
#include "ModelStuctures.h"
#include "Material.h"

class Mesh
{
private:
	GLuint vao;
	GLuint *locations;

	glm::mat4 P, V, M;
	glm::vec4 lightPos;

	GLuint locP, locV, locM, locLighPos, locEnableTexturing;

	string name;
	int segmentsCount;
	MeshSegment *segments;
	Material *materials;

	Texture *textures;
	unsigned textureCount;
	int enableTexturing;

	void SetupUniformVariables(int index);

public:

	Mesh();
	Mesh(vector<MeshSegment> ms, string name);
	~Mesh(void);

	void Update(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos);

	void Draw();

	void SetVao(GLuint vao){ this ->vao = vao; };
	
	void SetMaterials(Material *materials, int materialCount);
	
	void SetLocations(GLuint locP, GLuint locV, GLuint locM, GLuint locLighPos, GLuint *locations){ 

		this ->locP = locP;
		this ->locV = locV;
		this ->locM = locM;
		this ->locLighPos = locLighPos;
		this ->locations = locations; 

	};

	MeshSegment *GetSegments(){ return segments; };
	int GetSegmentCount(){ return segmentsCount; };

	string GetName(){ return name; };
	MeshSegment GetSegment(string name);

	glm::mat4 GetModelMatrix(){ return M; };

	void SetTextures(Texture *textures, unsigned textureCount, GLuint locEnableTexturing);

};
