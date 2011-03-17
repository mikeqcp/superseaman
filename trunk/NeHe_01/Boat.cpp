#include "StdAfx.h"
#include "Boat.h"
#include <math.h>

#define SIDE_RESISTANCE 0.5;	//wspó³czynnik oporu miecza

Boat::Boat(void)
{
}

Boat::~Boat(void)
{
}

Vector3D *Boat::getForwardForce()
{
	GLfloat length = sailForce * cos(boomAngle);

	Vector3D *result =new Vector3D(direction);
	result->scaleTo(length);

	return result;
}

Vector3D *Boat::getSideForce()
{
	GLfloat length = sailForce * sin(boomAngle);

	Vector3D *result = new Vector3D(direction.y, -direction.x, direction.z);
	if(boomAngle<0) result->negate();

	return result;
}

void Boat::updateAxis()
{
	axisAngle += (axisAngle>0 ? 1 : -1) * cos(axisAngle) * getSideForce()->length() * SIDE_RESISTANCE;
}