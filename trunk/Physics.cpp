#include "Physics.h"

Physics *Physics::instance_pointer = 0;

Physics::Physics(void)
{
}

Physics::~Physics(void)
{
}

Physics * Physics::instance()	//metoda dostêpu do klasy
{
	if(!instance_pointer)
		return new Physics();
	else
		return instance_pointer;
}

void Physics::initialize(Boat *boat)
{
	sailAngle = boat->getSailAngle();
	wind = glm::vec4(0);
	speed = glm::vec4(0);
}