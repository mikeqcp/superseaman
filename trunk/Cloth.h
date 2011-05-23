#pragma once

#include <vector>

#include "Model.h"
#include "includes.h"

class Cloth : public Model
{
private:
	glm::vec4 gravity, wind;

	glm::vec4 *oldVertices;
	glm::vec4 *verticesAcc;
	glm::vec4 *tempNormals;
    GLfloat *constraints;
	int *masses;

	int pinMaterialIndex, bompinMaterialIndex;

	void Verlet();
	void SatisfyConstraints();
    void AccumulateForces();

	void UpdateVBO();

public:
	Cloth(string fileName, glm::mat4 P, glm::mat4 V, glm::mat4 M, string vshaderFile, string fshaderFile);
	~Cloth(void);

	void BuildCloth();

	void TimeStep();

	void SetWind(glm::vec4 wind){ this ->wind = wind; };

	void CalculateNormals();

};

