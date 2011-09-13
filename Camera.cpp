#include "Camera.h"


Camera::~Camera(void)
{
}

Camera::Camera(SailingObject *target, glm::mat4 mat)
{
	initV = mat;
	V = glm::mat4(1);
	prevV = glm::mat4(1);
	this->target = target;
}

glm::mat4 Camera::update()
{
	glm::mat4 diff = V * glm::inverse(prevV);
	//glm::mat4 newV = glm::translate(glm::mat4(1), glm::vec3(-0.01,0 , 0)) *diff;
	glm::mat4 newV = glm::inverse(target->getCurrentPosition().translation);

	prevV = V;
	V = newV;

	return initV *newV;
}