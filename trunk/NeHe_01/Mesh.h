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
	Texture * textureCoords;

	int noVertices;
	int noNormals;

	Face *faces;
	unsigned noFaces;

	GLuint meshCL;

public:
	Mesh(vector<Face> facesVector, string name);

	~Mesh(void);

	void Draw();

	void UpdateVerticesNormalsTextures(Vertex *vertices, Vertex *normals, Texture * textureCoords);

	void BuildLists();

};

