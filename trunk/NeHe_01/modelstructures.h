#pragma once

#include <vector>
using namespace std;


struct Vertex{

	GLfloat x, y, z;

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