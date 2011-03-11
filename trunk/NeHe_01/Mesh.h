#pragma once
#include "modelstructures.h"
#include <vector>

using namespace std;

class Mesh
{
private:
	
	string name;

	Vertex *vertices;
	Vertex *normals;
	int noVertices;
	int noNormals;

	Face *faces;
	unsigned noFaces;

	GLuint meshCL;

public:
	Mesh(vector<Face> facesVector, string name);

	~Mesh(void);

	void Draw();

	void UpdateVerticesNormals(Vertex *vertices, Vertex *normals);

	void BuildLists();

};

