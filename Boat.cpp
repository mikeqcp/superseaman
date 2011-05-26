#include "Boat.h"


Boat::Boat(Model *boat, Cloth *sail): boat(boat), sail(sail){

	//
	sail->BuildCloth();

	//wyznaczenie ko�ca bomu, wg kt�rego b�dzie obracany on i �agiel
	int length;
	glm::vec4 *ms = boat->GetSegment("bom", "bomend", &length);
	bomEnd = glm::vec4(0);
	for(int i = 0; i < length; i++)
		bomEnd += ms[i];
	bomEnd /= length;

	//stworzenie macierzy, kt�ra przesuwa bom z pocz�tku uk�adu wsp�rz�dnych (punkt O) do jego pierwotnego miejsca
	//jej odwrotno�� oczywi�cie przesuwa bom z pierwotnego miejsca do punktu O
	translateBomMatrix = glm::translate(glm::mat4(1), glm::vec3(bomEnd.x, bomEnd.y, bomEnd.z));

}


Boat::~Boat(void)
{
}

void Boat::Draw(){

	boat -> Draw();
	sail -> Draw();

	sailAngle = 0;

}

void Boat::Update(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos){

	//aktualizacja macierzy kadlubu przed wykonaniem pozostalych operacji - zanim macierz M zostanie zmieniona dla zagla i bomu
	boat ->Update(P, V, M, lightPos);
	
	//stworz macierz rotacji, kt�ra obr�ci �agiel o k�t sailAngle
	glm::mat4 rotMat = glm::rotate(glm::mat4(1), sailAngle, glm::vec3(0, 1, 0));

	//przesun bom do pocz�tku uk�adu wsp�rz�dnych, obr�c o k�t sailAngle, przesu� z powrotem i przenie� do uk�adu �wiata
	M = M*translateBomMatrix*rotMat*glm::inverse(translateBomMatrix); 
	
	//Aktualizacja kadlubu i �agla
	boat->UpdateMesh("bom", P, V, M, lightPos);
	sail ->Update(P, V, M, lightPos);
	sail ->RotateWind(-sailAngle);

	//Krok czasowy dla metody ca�kowania verleta i funkcji spe�niania ogranicze�
	sail->TimeStep();

}

void Boat::RotateSail(GLfloat angle){

	sailAngle = angle;

	//oganiczenie obrotu zagla od -60 do 60 stopni
	if(sailAngle > 60) sailAngle = 60;
	else if(sailAngle < -60) sailAngle = -60;

}

void Boat::DrawReflection(){

	boat -> DrawReflection();
	sail -> DrawReflection();

}

void Boat::SetClipPlane(glm::vec4 clipPlane){

	boat->SetClipPlane(clipPlane);
	sail->SetClipPlane(clipPlane);

}

void Boat::SetTextures(Texture *textures, int texCount){

	boat ->SetTextures(textures, texCount);
	sail ->SetTextures(textures, texCount);

}