#include "StdAfx.h"
#include "Vector3D.h"
#include <math.h>

#pragma region initialization
Vector3D::Vector3D(void) 
{
   this->x=0;
   this->y=0;
   this->z=0;
}

Vector3D::Vector3D(GLfloat x, GLfloat y, GLfloat z) 
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
#pragma endregion

#pragma region overloads

Vector3D Vector3D::operator+(Vector3D v) 
{
 return Vector3D((x + v.x), (y + v.y), (z + v.z));
}

Vector3D Vector3D::operator-(Vector3D v) {
 return Vector3D((x - v.x), (y - v.y), (z - v.z));
}

Vector3D Vector3D::operator+(GLfloat w) 
{
 return Vector3D((x + w), (y + w), (z + w));
}

Vector3D Vector3D::operator-(GLfloat w) 
{
 return Vector3D((x - w), (y - w), (z - w));
}

Vector3D Vector3D::operator*(GLfloat w) 
{
 return Vector3D((x * w), (y * w), (z * w));
}

Vector3D Vector3D::operator/(GLfloat w) 
{
 return Vector3D((x / w), (y / w), (z / w));
}

Vector3D Vector3D::operator+=(Vector3D v){
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3D Vector3D::operator-=(Vector3D v){
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector3D Vector3D::operator*=(Vector3D v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}

Vector3D Vector3D::operator/=(Vector3D v){
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	return *this;
}

Vector3D Vector3D::operator+=(GLfloat w){
	this->x += w;
	this->y += w;
	this->z += w;
	return *this;
}

Vector3D Vector3D::operator-=(GLfloat w){
	this->x -= w;
	this->y -= w;
	this->z -= w;
	return *this;
}

Vector3D Vector3D::operator*=(GLfloat w){
	this->x *= w;
	this->y *= w;
	this->z *= w;
	return *this;
}

Vector3D Vector3D::operator/=(GLfloat w){
	this->x /= w;
	this->y /= w;
	this->z /= w;
	return *this;
}

bool Vector3D::operator==(Vector3D v){

	if(this ->x == v.x && this->y == v.y && this->z == v.z)
		return true;

	return false;

}

bool Vector3D::operator==(GLfloat w){

	if(this ->x == w && this->y == w && this->z == w)
		return true;

	return false;

}

bool Vector3D::operator!=(Vector3D v){

	if(this ->x != v.x || this->y != v.y || this->z != v.z)
		return true;

	return false;

}

bool Vector3D::operator!=(GLfloat w){

	if(this ->x != w || this->y != w || this->z != w)
		return true;

	return false;

}

#pragma endregion

void Vector3D::negate() 
{
 this ->x *=-1;
 this ->y *=-1;
 this ->z *=-1;
}

GLfloat Vector3D::length() 
{
 return sqrt((x * x) + (y * y) + (z * z));	//do zoptymalizowania
}

void Vector3D::normalize() 
{
	GLfloat l = length();
	if(l == 0) return;
	x /= l;
	y /= l;
	z /= l;
}

Vector3D* Vector3D::crossWith(Vector3D *b)
{
	Vector3D *a = this;

	GLfloat i = a->y*b->z - a->z*b->y; //tutaj byly inty, jesli rzeczywiscie maja byc to dodaj jawne rzutowanie
	GLfloat j = a->z*b->x - a->x*b->z;
	GLfloat k = a->x*b->y - a->y*b->x;

	return new Vector3D(i, j, k);
}

void Vector3D::scaleTo(GLfloat l)
{
	GLfloat div = l/length();

	this->x*=div;
	this->y*=div;
	this->z*=div;
}

void Vector3D::assign(GLfloat x, GLfloat y, GLfloat z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


GLfloat Vector3D::getHorizontalAngle(Vector3D *a)
{
	return abs(atan(this->y/this->x) - atan(a->y/a->x));
}

GLfloat Vector3D::getVerticalAngle(Vector3D *a)
{
	return abs(atan(this->z/this->x) - atan(a->z/a->x));
}

Vector3D *Vector3D::rotX(GLfloat alfa)
{
	Vector3D *patt = new Vector3D(x, y*cos(alfa)+z*sin(alfa), z*cos(alfa)-y*cos(alfa));
	
	assign(patt->x, patt->y, patt->z);
	return this;

}

Vector3D *Vector3D::rotY(GLfloat alfa)
{
	Vector3D *patt = new Vector3D(x*cos(alfa)-z*sin(alfa), y, x*sin(alfa)+z*cos(alfa));
	
	assign(patt->x, patt->y, patt->z);
	return this;

}

Vector3D *Vector3D::rotZ(GLfloat alfa)
{
	Vector3D *patt = new Vector3D(x*cos(alfa)+y*sin(alfa), y*cos(alfa)-x*sin(alfa), z);
	
	assign(patt->x, patt->y, patt->z);
	return this;

}

