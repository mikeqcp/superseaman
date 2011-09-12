#pragma once

#include "includes.h"
#include "Result.h"

namespace MyMaths
{
	glm::vec4 vecMin(glm::vec4 v1, glm::vec4 v2);	//zwraca wektor minimalnych sk³adowych z pary
	glm::vec4 vecMax(glm::vec4 v1, glm::vec4 v2);	//zwraca wektor maksymalnych sk³adowych z pary
	
	GLint signOf(GLfloat in);
}

enum Floating { ON, OFF };	//unoszenie siê na wodzie

class PhysicalObject
{
public:
	PhysicalObject(GLfloat mass);	//nowy obiekt
	PhysicalObject(GLfloat mass, glm::vec4 cenOfGrav);
	~PhysicalObject(void);

	void setForce(glm::vec4 f) { forceOuter = f; };
	void setFloating(Floating in) { floating = in; };
	virtual Result update();

protected:
	glm::vec4 gravity;
	GLfloat timeStep;

	GLfloat axisAngle ;	//k¹t nachylenia masztu

	Floating floating;	//si³a wyporu (ON/OFF)
	glm::vec4 floatForce;	//si³a wyporu(wartoœæ, zawsze pionowo w górê)

	glm::vec4 centerOfGravity;	//wzglêdem œrodka uk³adu wspó³rzêdnych modelu

	glm::mat4 M;	//pozycja w swiecie


	glm::vec4 forceOuter;	//dzia³aj¹ca si³a zewnêtrzna
	glm::vec4 forcesSum;	//suma si³

	const GLfloat mass;

	void satisfyConstraints();
	void init();
	glm::vec4 getFloatingForce();

	Result updateVertical();

private:

	glm::vec4 posCurr;
	glm::vec4 posPrev;

};