#pragma once

#include "includes.h"
#include "Boat.h"

class Physics
{
public:
	static Physics *instance();	//zwraca wska�nik (je�li trzeba tworzy obiekt klasy)
	
	void initialize(Boat *boat);	//ustawia dane otoczenia

protected:
	Physics(void);
	~Physics(void);

private:
	static Physics *instance_pointer;	//wska�nik na instacje singletona

	glm::vec4 speed;	//predkosc ��dki
	glm::vec4 wind;	//wiatr
	GLfloat sailAngle;	//k�t bomu (wzgl�dem osi ��dki)
	
};

