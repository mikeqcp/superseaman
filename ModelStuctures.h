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
	GLuint texture;

};

struct MaterialStruct{

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	int Ns;

};