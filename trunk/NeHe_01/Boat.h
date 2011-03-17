#include "Vector3D.h"

#pragma once
class Boat
{
private:
	GLfloat axisAngle;	//odchylenie od pionu

public:
	Vector3D direction;	//always z=0
	Vector3D mastAxis;	//jeszcze nie wiem czy potrzebne

	GLfloat sailForce;	//force on the sail (onto the sail)
	GLfloat boomAngle;	//from direction axis, counterclockwise

	Boat(void);
	~Boat(void);

	Vector3D *getForwardForce();
	Vector3D *getSideForce();
	void updateAxis();
};

