#pragma once
#include "includes.h"
#include "SailingObject.h"
class Camera
{
public:
	~Camera(void);
	Camera(SailingObject *target, glm::mat4 mat);

	//glm::mat4 getViewMatrix() {return V;};
	glm::mat4 update();

private:
	glm::mat4 V, prevV;
	glm::mat4 initV;
	SailingObject *target;
};

