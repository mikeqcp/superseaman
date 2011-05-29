#pragma once

#include "includes.h"
#include "Boat.h"
#include "PhysicalObject.h"
#include <cmath>

class Physics
{
public:
	static Physics *instance();	//zwraca wska�nik (je�li trzeba tworzy obiekt klasy)
	
	glm::vec4 getWind() { return wind; };
	glm::mat4 getWindScaleMatrix() { return glm::scale(glm::mat4(1), glm::vec3(windStr, windStr, windStr)); };
	glm::mat4 getWindMatrix() { return windM; };

	void setTargets(PhysicalObject *objArray, int size);
	void update();

protected:
	Physics(void);
	~Physics(void);

private:
	static Physics *instance_pointer;	//wska�nik na instacje singletona

	PhysicalObject *targets;
	int targetNum;

	glm::vec4 speed;	//predkosc ��dki

	glm::vec4 wind;	//wiatr
	glm::mat4 windM;	//macierz transformacji wiatru (dla strza�ki)
	GLfloat windStr;	//si�a wiatru

	GLfloat sailAngle;	//k�t bomu (wzgl�dem osi ��dki)
	
	void updateWind();
	
};


