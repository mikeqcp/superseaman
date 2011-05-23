#include "Boat.h"


Boat::Boat(Model *boat, Cloth *sail): boat(boat), sail(sail){

	sail->BuildCloth();

	int length;
	glm::vec4 *ms = boat->GetSegment("bom", "bomend", &length);
	bomEnd = glm::vec4(0);
	for(int i = 0; i < length; i++)
		bomEnd += ms[i];
	bomEnd /= length;

}


Boat::~Boat(void)
{
}

void Boat::Draw(){

	boat -> Draw();
	sail ->Draw();

	sailAngle = 0;

}

void Boat::Update(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos){

	boat ->Update(P, V, M, lightPos);
	
	M = glm::translate(glm::rotate(glm::translate(M, glm::vec3(bomEnd.x, bomEnd.y, bomEnd.z)), sailAngle, glm::vec3(0, 1, 0)), glm::vec3(-bomEnd.x, -bomEnd.y, -bomEnd.z));
	boat->UpdateMesh("bom", P, V, M, lightPos);
	sail ->Update(P, V, M, lightPos);
	sail ->RotateWind(-sailAngle);
	sail->TimeStep();
}

void Boat::RotateSail(GLfloat angle){

	sailAngle = angle;

	if(sailAngle > 60) sailAngle = 60;
	else if(sailAngle < -60) sailAngle = -60;
}