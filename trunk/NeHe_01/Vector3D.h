#pragma once
class Vector3D
{
private:

public:
	GLfloat x, y, z;	//coordinates

	Vector3D(void);
	Vector3D::Vector3D(GLfloat a, GLfloat b, GLfloat c);
	Vector3D::Vector3D(const Vector3D &c);
	~Vector3D(void);

	Vector3D operator+(Vector3D v);
	Vector3D operator-(Vector3D v);

	Vector3D operator+(GLfloat v);
	Vector3D operator-(GLfloat v);
	Vector3D operator*(GLfloat w);
	Vector3D operator/(GLfloat w);

	Vector3D operator+=(Vector3D v);
	Vector3D operator-=(Vector3D v);
	Vector3D operator*=(Vector3D v);
	Vector3D operator/=(Vector3D v);

	Vector3D operator+=(GLfloat w);
	Vector3D operator-=(GLfloat w);
	Vector3D operator*=(GLfloat w);
	Vector3D operator/=(GLfloat w);

	bool operator==(Vector3D v);
	bool operator==(GLfloat w);
	bool operator!=(Vector3D v);
	bool operator!=(GLfloat w);

	void assign(GLfloat x, GLfloat y, GLfloat z);

	void negate();
	GLfloat length();
	void normalize();
	void scaleTo(GLfloat length);	//wyd³u¿a wektor do podanej d³ugoœci, wzdluz aktualnego kierunku
	Vector3D *crossWith(Vector3D *v);

	Vector3D *rotX(GLfloat angle);
	Vector3D *rotY(GLfloat angle);
	Vector3D *rotZ(GLfloat angle);
};

