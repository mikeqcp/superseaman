#pragma once

#include <vector>
using namespace std;

struct Vertex{

	GLdouble x, y, z;

};

struct Texture{

	GLdouble u, v;

};

struct Face{

	vector<int > vertices;
	vector<int > normalIndex;
	vector<int > textures;

};