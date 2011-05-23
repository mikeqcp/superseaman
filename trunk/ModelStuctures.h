#pragma once

#include "includes.h"

using namespace std;

struct Texture{

	GLuint tex;
	string name;

};

struct MeshSegment{

	string materialName;
	int begin;
	int vertexCount;
	int materialIndex;

};