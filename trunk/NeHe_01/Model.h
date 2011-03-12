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
	Texture *textureCoordsTab;

public:

	Model();

	Model(string fileName);

	~Model(void);

	void Load(string fileName);

	vector<Mesh> meshes;
};

