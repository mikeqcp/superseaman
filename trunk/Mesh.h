#pragma once

#include <vector>
#include "ModelStuctures.h"
#include "Material.h"

class Mesh
{
private:
	GLuint vao;
	GLuint *locations;

	string name;
	int segmentsCount;
	MeshSegment *segments;
	Material *materials;

	void SetupUniformVariables(int index);

public:

	Mesh();
	Mesh(vector<MeshSegment> ms, string name);
	~Mesh(void);

	void Update();

	void Draw();

	void SetVao(GLuint vao){ this ->vao = vao; };
	
	void SetMaterials(Material *materials, int materialCount);
	
	void SetLocations(GLuint *locations){ this ->locations = locations; };

	MeshSegment *GetSegments(){ return segments; };
	int GetSegmentCount(){ return segmentsCount; };

};
