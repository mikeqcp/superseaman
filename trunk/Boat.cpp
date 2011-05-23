#include "Boat.h"


Boat::Boat(Model *boat, Cloth *sail): boat(boat), sail(sail){

	sail->BuildCloth();
}


Boat::~Boat(void)
{
}

void Boat::Draw(){

	boat -> Draw();
	sail ->Draw();

}

void Boat::Update(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos){

	boat ->Update(P, V, M, lightPos);
	sail ->Update(P, V, M, lightPos);
	sail->TimeStep();

}