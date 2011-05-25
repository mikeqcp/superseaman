#pragma once

#include "includes.h"
#include "Boat.h"

class Physics
{
public:
	static Physics *instance();	//zwraca wskaünik (jeúli trzeba tworzy obiekt klasy)
	Physics(void);
	~Physics(void);

	
	void initialize(Boat *boat);	//ustawia dane otoczenia

private:
	static Physics *instance_pointer;	//wskaünik na instacje singletona

	glm::vec4 speed;	//predkosc ≥Ûdki
	glm::vec4 wind;	//wiatr
	GLfloat sailAngle;	//kπt bomu (wzglÍdem osi ≥Ûdki)
	
};

