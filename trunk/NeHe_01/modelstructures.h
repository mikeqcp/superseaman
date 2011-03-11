#pragma once

#include <vector>
using namespace std;

struct Vertex{

	GLdouble x, y, z;

};

struct Face{

	vector<int > vertices;
	vector<int > normalIndex;

};