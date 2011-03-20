#pragma once
#include "modelstructures.h"
#include <vector>
#include "Material.h"

using namespace std;

class Mesh
{
private:
	
	string name;

	Vector3D *vertices;
	Vector3D *normals;
	Texture *textures;
	TextureUV * textureCoords;
	Material *materials;

	int noMaterials;
	int noTextures;
	short *dividers;

	Face *faces;
	unsigned noFaces;
	unsigned noVertices;

	GLuint meshCL;
	GLuint currentCL;

	GLuint directMeshCL;
	bool directMeshCLcreated;
	bool normalsInitialized;

	int noKeyFrames;
	int currentKeyFrame;

	void rotate(GLfloat angle, Vector3D v);

public:
	Mesh(vector<Face> facesVector, string name);

	~Mesh(void);

	string getName(){ return name; }

	void Draw();

	void NextKeyFrame();

	void DirectDraw(bool showNormals);

	void UpdateVerticesNormalsTexturesMaterials(Vector3D *vertices, int noVertices, Vector3D *normals, Texture *textures, int textureCount, TextureUV * textureCoords, Material *materials, int mCount);

	void BuildLists();

	vector<Edge> createEdgeList(int noVertices);

	vector<int> getOutline();

	vector<int> getBomPins();

	void ComputeNormals();

	void createRotation(Vector3D v);


};

