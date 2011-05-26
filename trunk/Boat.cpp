#include "Boat.h"


Boat::Boat(Model *boat, Cloth *sail): boat(boat), sail(sail){

	//
	sail->BuildCloth();

	//wyznaczenie koñca bomu, wg którego bêdzie obracany on i ¿agiel
	int length;
	glm::vec4 *ms = boat->GetSegment("bom", "bomend", &length);
	bomEnd = glm::vec4(0);
	for(int i = 0; i < length; i++)
		bomEnd += ms[i];
	bomEnd /= length;

	//stworzenie macierzy, która przesuwa bom z pocz¹tku uk³adu wspó³rzêdnych (punkt O) do jego pierwotnego miejsca
	//jej odwrotnoœæ oczywiœcie przesuwa bom z pierwotnego miejsca do punktu O
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
	
	//stworz macierz rotacji, która obróci ¿agiel o k¹t sailAngle
	glm::mat4 rotMat = glm::rotate(glm::mat4(1), sailAngle, glm::vec3(0, 1, 0));

	//przesun bom do pocz¹tku uk³adu wspó³rzêdnych, obróc o k¹t sailAngle, przesuñ z powrotem i przenieœ do uk³adu œwiata
	M = M*translateBomMatrix*rotMat*glm::inverse(translateBomMatrix); 
	
	//Aktualizacja kadlubu i ¿agla
	boat->UpdateMesh("bom", P, V, M, lightPos);
	sail ->Update(P, V, M, lightPos);
	sail ->RotateWind(-sailAngle);

	//Krok czasowy dla metody ca³kowania verleta i funkcji spe³niania ograniczeñ
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