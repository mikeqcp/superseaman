#pragma once

#include <vector>
using namespace std;


struct Edge{

	int v1, v2;

};

struct TextureUV{

	GLfloat u, v;

};

struct Texture{

	GLuint tex;
	string name;

};

struct Face{

	vector<int > vertices;
	vector<int > normalIndex;
	vector<int > textures;
	string materialName;

};