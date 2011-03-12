#pragma once

#include <string>
#include <vector>
#include "Mesh.h"
#include "modelstructures.h"

using namespace std;

class Model
{
private:
	Vertex *verticesTab;
	Vertex *normalsTab;
	TextureUV *textureCoordsTab;
	Material *materials;
	Texture *textures;

	int textureCount;

public:

	Model();

	Model(string fileName, Texture *textures, int textureCount);

	~Model(void);

	void Load(string fileName, Texture *textures, int textureCount);

	vector<Mesh> meshes;
};

