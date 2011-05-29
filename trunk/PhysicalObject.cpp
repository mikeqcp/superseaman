#include "PhysicalObject.h"

static glm::vec4 vecMin(glm::vec4 v1, glm::vec4 v2)
{
	glm::vec4 min = v1;

	if(v2.x<v1.x) min.x = v2.x;
	if(v2.y<v1.y) min.x = v2.y;
	if(v2.z<v1.z) min.x = v2.z;
}

static glm::vec4 vecMax(glm::vec4 v1, glm::vec4 v2)
{
	glm::vec4 min = v1;

	if(v2.x>v1.x) min.x = v2.x;
	if(v2.y>v1.y) min.x = v2.y;
	if(v2.z>v1.z) min.x = v2.z;
}

PhysicalObject::PhysicalObject(GLfloat massVal) :mass(massVal)
{
	M = glm::mat4(1);
	timeStep = 0.01f;

	gravity = glm::vec4(0,-4, 0, 0);
	force = glm::vec4(0, 0, 0, 1);

	posCurr = glm::vec4(0);
	posPrev = glm::vec4(0);


}

PhysicalObject::~PhysicalObject(void)
{
}

Result PhysicalObject::update()
{
	Result res;
	glm::vec4 newPos;

	newPos = posCurr + posCurr - posPrev + mass * timeStep* timeStep * (force + gravity);

	posPrev = posCurr;
	posCurr = newPos;

	res.translation = glm::translate(M, glm::vec3(posCurr));

	return res;
}

void PhysicalObject::satisfyConstraints()
{

}