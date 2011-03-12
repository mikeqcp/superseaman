#pragma once
#include "modelstructures.h"
#include <vector>
#include "Material.h"

using namespace std;

class Mesh
{
private:
	
	string name;

	Vertex *vertices;
	Vertex *normals;
	Texture *textures;
	TextureUV * textureCoords;
	Material *materials;

	int noMaterials;
	int noTextures;

	Face *faces;
	unsigned noFaces;

	GLuint meshCL;

public:
	Mesh(vector<Face> facesVector, string name);

	~Mesh(void);

	void Draw();

	void UpdateVerticesNormalsTexturesMaterials(Vertex *vertices, Vertex *normals, Texture *textures, int textureCount, TextureUV * textureCoords, Material *materials, int mCount);

	void BuildLists();

};

