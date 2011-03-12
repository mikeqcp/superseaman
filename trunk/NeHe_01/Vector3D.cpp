#include "StdAfx.h"
#include "Vector3D.h"

Vector3D::Vector3D(void) 
{
   this->x=0;
   this->y=0;
   this->z=0;
}

Vector3D::Vector3D(double x, double y, double z) 
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3D::Vector3D(const Vector3D &c) 
{
	this->x = c.x;
	this->y = c.y;
	this->z = c.z;
}

Vector3D::~Vector3D(void)
{
}
