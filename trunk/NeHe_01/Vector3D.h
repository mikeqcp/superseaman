#pragma once
class Vector3D
{
private:
	double x, y, z;	//coordinates
public:
	Vector3D(void);
	Vector3D::Vector3D(double a, double b, double c);
	Vector3D::Vector3D(const Vector3D &c);
	~Vector3D(void);
};

