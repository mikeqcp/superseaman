#pragma once

#include "includes.h"
#include "Boat.h"
#include "PhysicalObject.h"
#include <cmath>

const double PI = 3.141592653589793;

class Physics
{
public:
	static Physics *instance();	//zwraca wskaünik (jeúli trzeba tworzy obiekt klasy)
	
	glm::vec4 getWind() { return wind; };
	glm::mat4 getWindScaleMatrix() 
	{ 
		return glm::scale(glm::mat4(1), glm::vec3(0.7,0.7,0.7));
	};
	glm::mat4 getWindMatrix() { return windM; };

	void setTargets(PhysicalObject **objArray, int size);
	PhysicalObject **getTargets(){return targets; };

	void update();
	Result *getStates();

protected:
	Physics(void);
	~Physics(void);

private:
	static Physics *instance_pointer;	//wskaünik na instacje singletona

	PhysicalObject **targets;
	Result *states;
	int targetNum;

	glm::vec4 speed;	//predkosc ≥Ûdki

	glm::vec4 wind;	//wiatr
	glm::mat4 windM;	//macierz transformacji wiatru (dla strza≥ki)
	GLfloat windStr;	//si≥a wiatru

	GLfloat sailAngle;	//kπt bomu (wzglÍdem osi ≥Ûdki)
	
	void updateWind();
	
};


GLfloat degToRad(GLfloat val);