#pragma once

#include <vector>
using namespace std;

struct Vertex{

	GLfloat x, y, z;

};

struct Face{

	vector<int > vertices;
	int normalIndex;

};