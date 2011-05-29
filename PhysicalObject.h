#pragma once

#include "includes.h"
#include "Result.h"

static glm::vec4 vecMin(glm::vec4 v1, glm::vec4 v2);	//zwraca wektor minimalnych sk�adowych z pary
static glm::vec4 vecMax(glm::vec4 v1, glm::vec4 v2);	//zwraca wektor maksymalnych sk�adowych z pary

class PhysicalObject
{
public:
	PhysicalObject(GLfloat mass);	//nowy obiekt
	~PhysicalObject(void);

	void setForce(glm::vec4 f) { force = f; };
	Result update();

private:

	glm::vec4 gravity;
	GLfloat timeStep;

	glm::mat4 M;	//pozycja w swiecie

	glm::vec4 posCurr;
	glm::vec4 posPrev;

	glm::vec4 force;	//dzia�aj�ca si�a

	const GLfloat mass;

	void satisfyConstraints();
};