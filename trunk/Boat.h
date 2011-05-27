#pragma once

#include "Cloth.h"


class Boat
{
private:

	Model *boat;
	Cloth *sail;

	GLfloat sailAngle;
	glm::vec4 bomEnd;
	glm::mat4 translateBomMatrix;

public:
	Boat(Model *boat, Cloth *sail);
	~Boat(void);

	void Draw();

	void DrawReflection();

	void Update(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos);

	void SetWind(glm::vec4 wind){ sail->SetWind(wind); };

	void RotateSail(GLfloat angle);

	void SetClipPlane(glm::vec4 clipPlane);

	GLfloat getSailAngle() { return sailAngle; }

	void SetTextures(Texture *textures, int texCount);

	void SetLookAtPos(glm::vec3 lookAtPos);
};

