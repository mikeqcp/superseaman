#pragma once

#include "glm\glm.hpp";

class Result
{
public:
	glm::mat4 rotation;
	glm::mat4 translation;
	glm::mat4 scale;

	Result(void);
	~Result(void);
};

