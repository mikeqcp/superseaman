#pragma once

#include <string>
#include <vector>
#include "Mesh.h"
#include "modelstructures.h"

using namespace std;

class Model
{
protected:
	Vertex *verticesTab;
	Vertex *normalsTab;
	TextureUV *textureCoordsTab;
	Material *materials;
	Texture *textures;

	int textureCount;
	int noVertices;

public:

	Model();

	Model(string fileName, Texture *textures, int textureCount);

	~Model(void);

	void Load(string fileName, Texture *textures, int textureCount);

	vector<Mesh> meshes;

	int VerticesCount(){ return noVertices; }

	vector<Edge> createEdgeList();

	vector<int> Model::getOutline();

	Vertex *getVertices(){ return verticesTab; }

};

