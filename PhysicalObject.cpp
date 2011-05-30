#include "PhysicalObject.h"
using namespace MyMaths;

glm::vec4 vecMin(glm::vec4 v1, glm::vec4 v2)
{
	glm::vec4 min = v1;

	if(v2.x<v1.x) min.x = v2.x;
	if(v2.y<v1.y) min.y = v2.y;
	if(v2.z<v1.z) min.z = v2.z;
	return min;
}

glm::vec4 vecMax(glm::vec4 v1, glm::vec4 v2)
{
	glm::vec4 max = v1;

	if(v2.x>v1.x) max.x = v2.x;
	if(v2.y>v1.y) max.y = v2.y;
	if(v2.z>v1.z) max.z = v2.z;

	return max;
}

PhysicalObject::PhysicalObject(GLfloat massVal) :mass(massVal)
{
	init();
}

PhysicalObject::PhysicalObject(GLfloat massVal, glm::vec4 cenOfGrav) :mass(massVal)
{
	init();
	centerOfGravity = cenOfGrav;
}

PhysicalObject::~PhysicalObject(void)
{
}

void PhysicalObject::init()
{
	M = glm::mat4(1);
	timeStep = 0.01f;

	gravity = glm::vec4(0,-4, 0, 0);
	forceOuter = glm::vec4(0, 0, 0, 1);

	posCurr = glm::vec4(0);
	posPrev = glm::vec4(0);
	centerOfGravity = glm::vec4(0,0,0,1);
}

Result PhysicalObject::update()
{
	Result res;
	glm::vec4 newPos;

	forcesSum = forceOuter + (floating ? floatForce : glm::vec4(0,0,0,0));

	newPos = posCurr + posCurr - posPrev + mass * timeStep* timeStep * (forcesSum + gravity);

	posPrev = posCurr;
	posCurr = newPos;

	res.translation = glm::translate(M, glm::vec3(posCurr));

	return res;
}

GLint MyMaths::signOf(GLfloat in)
{
	 return (in<0 ? -1 : (in>0 ? 1 : 0));
}

glm::vec4 PhysicalObject::getFloatingForce()
{
	glm::vec4 cenPos = M * centerOfGravity;
	GLfloat force = glm::length(cenPos);

	force =  -1 * (signOf(cenPos.y) * force *force);

	floatForce = glm::inverse(M) * glm::vec4(0,force, 0, 1);
	return floatForce;
}

void PhysicalObject::satisfyConstraints()
{

}