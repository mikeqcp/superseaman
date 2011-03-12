#pragma once

#include "Vector3D.h";

//Singleton
class Phisics
{
private:
	Vector3D windDirection;	//direction of the wind
		double windStrenght;	//strength of the wind

	Vector3D boatDirection;	//direction of the movement
		double speed;	//speed of the movement
	Vector3D boatAngle;	//describes the axis of the mast
	double boatMass;
	double boomAngle;	//boom rotation angle


public:
	Phisics(void);
	~Phisics(void);
};