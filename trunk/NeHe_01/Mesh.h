#pragma once
#include "modelstructures.h"
#include <vector>

using namespace std;

class Mesh
{
private:
	
	string name;

	vector<Vertex> *vertices;
	vector<Vertex> *normals;

	Face *faces;
	unsigned noFaces;

public:
	Mesh(vector<Vertex> *verticesVector, vector<Vertex> *normalsVector, vector<Face> facesVector, string name);

	~Mesh(void);

	void Draw();

};

