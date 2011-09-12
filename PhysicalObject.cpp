#include "PhysicalObject.h"
#include "Physics.h"
#include <math.h>
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
	floating = ON;

	posCurr = glm::vec4(0);
	posPrev = glm::vec4(0);
	centerOfGravity = glm::vec4(0,0,0,1);
}

Result PhysicalObject::update()
{
	return updateVertical();
}

Result PhysicalObject::updateVertical()
{
	Result res;
	glm::vec4 newPos, newPos_temp;
	
	forcesSum = forceOuter + (floating==ON ? getFloatingForce() : glm::vec4(0,0,0,0));

	newPos_temp = posCurr + posCurr - posPrev + mass * timeStep* timeStep * (forcesSum + gravity);

	newPos = newPos_temp;

	posPrev = posCurr;
	posCurr = newPos;

	res.translation = glm::translate(glm::mat4(1), glm::vec3(newPos.x, newPos.y, newPos.z));

	return res;
}

GLint MyMaths::signOf(GLfloat in)
{
	 return (in<0 ? -1 :  1 );
}

glm::vec4 PhysicalObject::getFloatingForce()
{
	
	//glm::vec4 cenPos = M * centerOfGravity;
	glm::vec4 cenPos = posCurr +  centerOfGravity;
	GLfloat force = glm::length(glm::vec3(cenPos.x, cenPos.y, cenPos.z));

	if(signOf(cenPos.y)<=0)
	{
		force =  3.5 * force *force;
		//force *= (2-cos(degToRad(axisAngle)));
	}
	else
	{
		force = 0;
	}

	//floatForce = glm::inverse(M) * glm::vec4(0,force, 0, 1);
	floatForce = glm::vec4(0,force, 0, 1);
	return floatForce;
	

	//return glm::vec4(0,4, 0, 1);
}

void PhysicalObject::satisfyConstraints()
{

}