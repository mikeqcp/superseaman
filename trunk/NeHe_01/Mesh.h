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
	unsigned noVertices;

	GLuint meshCL;

	GLuint directMeshCL;
	bool directMeshCLcreated;
	bool normalsInitialized;

public:
	Mesh(vector<Face> facesVector, string name);

	~Mesh(void);

	void Draw();

	void DirectDraw(bool showNormals);

	void UpdateVerticesNormalsTexturesMaterials(Vertex *vertices, int noVertices, Vertex *normals, Texture *textures, int textureCount, TextureUV * textureCoords, Material *materials, int mCount);

	void BuildLists();

	vector<Edge> createEdgeList(int noVertices);

	vector<int> getOutline();

	void ComputeNormals();

};

