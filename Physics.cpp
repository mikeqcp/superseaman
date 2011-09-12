#include "Physics.h"
#include <cstdlib>
#include <time.h>

#define RAND ((GLfloat)((rand()%10000)-4500)/10000)

Physics *Physics::instance_pointer = 0;

Physics::Physics(void)
{
	srand(time(NULL));

	sailAngle = 0;
	wind = glm::vec4(0, 0, 10, 1);
	speed = glm::vec4(1,0,0,1);
	windStr = 1.0f;
	windM = glm::mat4(1);
}

Physics::~Physics(void)
{
}

Physics *Physics::instance()	//metoda dostêpu do klasy
{
	if(!instance_pointer)
	{
		instance_pointer = new Physics();
	}
	
	return instance_pointer;
}

GLfloat degToRad(GLfloat val)
{
	return val * PI/180;
}

void Physics::update()	//odœwie¿a œrodowisko
{
	updateWind();

	if(!targets) return;
	for(int i=0; i<targetNum; i++)
	{
		states[i] = targets[i]->update();
	}
}

//TODO nadal nienaturalnie sie obraca w jedn¹ strone
//a si³a tylko roœnie
void Physics::updateWind()	//odpowiedzialne za wianie wiatru (kierunek i si³a)
{
	int mult = 1;
	GLfloat temp;

	temp = RAND;

	if(temp<0.02 && temp>=0) mult = 50;	//nag³e zmiany

	temp = RAND;
	glm::mat4 transform = glm::rotate(glm::mat4(1), temp*mult, glm::vec3(0,1,0));	//obrót wiatru

	
	try	//zmiana si³y (sprawdzenie czy nie przekracza dopuszczalnych norm)
	{
		temp =(1 + (RAND/(100)*mult));
		glm::vec4 windTemp = glm::scale(glm::mat4(1), glm::vec3(temp, temp, temp))*wind;

		temp = glm::length(glm::vec3(windTemp.x, windTemp.y, windTemp.z));

		if(temp <5 || temp>15)
			throw "power extending bounds";
		else 
		{
			wind = windTemp;
			windStr = temp;
			wind.w = windStr;
		}
	}
	catch (...){}
	
	
	//cout << wind.x << '\t' << wind.y << '\t' << wind.z << '\t' << wind.w << "\n";
	//cout << windStr << "\n";

	wind = transform * wind;
	//windM = glm::scale(glm::mat4(1), glm::vec3(windStr, 1, 1)) * transform * windM;
	windM = transform * windM;

}

void Physics::setTargets(PhysicalObject **objArray, int size)
{
	targets = objArray;
	targetNum = size;
	states = new Result[size];

	for(int i=0; i<size; i++)
	{
		states[i].rotation = glm::mat4(1);
		states[i].scale = glm::mat4(1);
		states[i].translation = glm::mat4(1);
	}
}

Result *Physics::getStates()
{
	return states;
}

