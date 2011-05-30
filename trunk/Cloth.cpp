#include "Cloth.h"
#include "glm\gtx\rotate_vector.hpp"

Cloth::Cloth(string fileName, glm::mat4 P, glm::mat4 V, glm::mat4 M, string vshaderFile, string fshaderFile) {
	
	this ->P = P;
	this ->V = V;
	this ->M = M;

	fTimeStep = 0.01f;

	gravity = glm::vec4(0, -4, 0, 1); //grawitacja jest sta�a, taka warto�� zachowuje wystarczaj�cy realizm
	wind = glm::vec4(10, 0, 0, 0); //domy�lny wiatr, tylko dla bezpiecze�stwa
	realWind = glm::vec4(10, 0, 0, 0); //prawdziwy wiatr jest to wiatr w przestrzeni �wiata, przed przekszta�ceniami

	SetupShaders(vshaderFile, fshaderFile);
	Load(fileName, 0, 0);
	verticesBufferType = GL_DYNAMIC_DRAW;
	SetupBuffers();

	for(int i = 0; i < materialCount; i++){

		string mname = materials[i].getName();
	
		if(mname.find("pins") != string::npos && mname.find("bompins") == string::npos)
			pinMaterialIndex = i;
		else if(mname.find("bompins") != string::npos)
			bompinMaterialIndex = i;

	}

}


Cloth::~Cloth(void)
{
}

/*
*
* G��wna metoda wywo�uj�ca poszczeg�lne etapy metody ca�kowania verleta
*
*/

void Cloth::TimeStep(){

	AccumulateForces();
	Verlet();
	SatisfyConstraints();
	CalculateNormals();
	UpdateVBO();

}

/*
*
* Alokowanie pami�ci dla algorytmu ca�kowania Verleta i funkcji spe�niania ogranicze�
* Ustalanie masy poszczeg�lnych punkt�w p��tna
* Masa 0 oznacza, �e wierzcho�ek pozostanie na swojej pozycji, a wi�c b�dzie punktem zaczepienia
* Punkty te s� rozpoznawane dzi�ki nazwie materialu do nich przypisanego - "pins" oraz "bompins", indeksy tych materia��w w tablicy 
* zostaly ustalone w konstruktorze
* Tutaj r�wnie� obliczane s� ograniczenia dla d�ugo�ci poszczeg�lnych kraw�dzi
*/

void Cloth::BuildCloth(){

	oldVertices = new glm::vec4[originalVerticesCount];
	verticesAcc = new glm::vec4[originalVerticesCount];
	masses = new int[originalVerticesCount];
	constraints = new GLfloat[indicesCount];
	tempNormals = new glm::vec4[originalVerticesCount];

	int pinInterval[2], bompinInterval[2];

	MeshSegment *ms = meshes[0].GetSegments();
	for(int i = 0; i < meshes[0].GetSegmentCount(); i++){
	
		if(ms[i].materialIndex == pinMaterialIndex){

			pinInterval[0] = ms[i].begin;
			pinInterval[1] = ms[i].begin + ms[i].vertexCount;

		}
		else if(ms[i].materialIndex == bompinMaterialIndex){

			bompinInterval[0] = ms[i].begin;
			bompinInterval[1] = ms[i].begin + ms[i].vertexCount;

		}

	}
	
	for(int i = 0; i < originalVerticesCount; i++){

		bool found = false;

		for(int j = pinInterval[0]*4; j < pinInterval[1]*4; j+=4){
		
			if(glm::vec4(vertices[j], vertices[j+1], vertices[j+2], vertices[j+3]) == originalVertices[i]){
				masses[i] = 0;
				found = true;
				break;
			}

		}

		if(!found)
		for(int j = bompinInterval[0]*4; j < bompinInterval[1]*4; j+=4){

			if(glm::vec4(vertices[j], vertices[j+1], vertices[j+2], vertices[j+3]) == originalVertices[i]){
				masses[i] = 0;
				found = true;
				break;
			}

		}

		if(!found)
			masses[i] = 1;
	
		oldVertices[i] = originalVertices[i];

	}

	
	for(int i = 0; i < indicesCount; i += 3){
	
		glm::vec4 v1 = originalVertices[indices[i]];
		glm::vec4 v2 = originalVertices[indices[i+1]];
		glm::vec4 v3 = originalVertices[indices[i+2]];

		constraints[i] = glm::length(v1-v2);
		constraints[i+1] = glm::length(v2-v3);
		constraints[i+2] = glm::length(v3-v1);
	}

}

/*
*
* Metoda sumuj�ca si�y pochodz�ce z r�znych �r�de�
*
*/

void Cloth::AccumulateForces(){

	for(int i = 0; i < originalVerticesCount; i++)
		verticesAcc[i] = gravity + wind;

}

/*
*
* Metoda ca�kowania Verleta
* Dzi�ki zapami�tanym warto�ciom poprzedniego po�o�enia, mozna pomin�� tak� wielko�� jak pr�dko��, kt�ra wprowadza zbyt du�� niedok�adno�� przy
* obliczaniu nowego po�o�enia
* W zamian za to do obecnego po�o�enia dodawana jest r�znica obecnego i poprzedniego
* Na ko�cu dodawany jest wektor si�y przemno�ony przez kwadrat ma�ego odcinka czasowego, wcze�niej ustalonego
* 
*/

void Cloth::Verlet(){

	glm::vec4 v, temp, old, a;
	for(int i = 0; i < originalVerticesCount; i++){

		v = originalVertices[i];
		temp = v;
		old = oldVertices[i];
		a = verticesAcc[i];

		v += v - old + a*fTimeStep*fTimeStep*(GLfloat)masses[i];

		v.w = originalVertices[i].w;

		originalVertices[i] = v;
		oldVertices[i] = temp;

	}

}

/*
*
* Ta metoda zapewnia utrzymanie odpowiednich odleg�o�ci mi�dzy wierzcho�kami, kt�re mog�y zosta� zaburzone w etapie poprzednim (verlet)
* 
*
*/

void Cloth::SatisfyConstraints(){

        int i = 0;
		int iv1, iv2;
		for(int j = 0; j < 2; j++){
			for(int i = 0; i < indicesCount; i++){
				
				iv1 = indices[i];
				if((i+1)%3 == 0)
					iv2 = indices[i-2];
				else iv2 = indices[i+1];
			
				glm::vec4 v1 = originalVertices[iv1];
				glm::vec4 v2 = originalVertices[iv2];

				glm::vec4 delta = v2 - v1;

				short m1 = masses[iv1];
				short m2 = masses[iv2];

				GLfloat deltalength = glm::length(delta);
				GLfloat diff = (deltalength - constraints[i])/deltalength;

				v1 += delta *0.5f*diff*(GLfloat)m1;
				v2 -= delta *0.5f*diff*(GLfloat)m2;

				originalVertices[iv1] = v1;
				originalVertices[iv2] = v2;
			}
		}



}

/*
*
* Ta metoda oblicza normalne dla nowych wierzcho�k�w p��tna.
* 
*
*/

void Cloth::CalculateNormals(){

	for(int i = 0; i < originalVerticesCount; i++)
		tempNormals[i] = glm::vec4(0);

	int iv1, iv2, iv3;
	glm::vec4 a, b, c, normal, U, V;
	for(int i = 0; i < indicesCount; i+=3){
	
		iv1 = indices[i];
		iv2 = indices[i+1];
		iv3 = indices[i+2];

		a = originalVertices[iv1];
		b = originalVertices[iv2];
		c = originalVertices[iv3];

		U = a - b;
		V = c - b;

		normal.x = U.y * V.z - U.z * V.y;
		normal.y = U.z * V.x - U.x * V.z;
		normal.z = U.x * V.y - U.y * V.x;

		normal = glm::normalize(normal);

		tempNormals[iv1] += normal;
		tempNormals[iv2] += normal;
		tempNormals[iv3] += normal;

	}

	for(int i = 0; i < originalVerticesCount; i++)
		tempNormals[i] = glm::normalize(tempNormals[i]);

}

void Cloth::UpdateVBO(){
	
	glm::vec4 v, n;
	for(unsigned i = 0; i < indicesCount; i++){

		v = originalVertices[indices[i]];
		vertices[i*4] = v.x;
		vertices[i*4 + 1] = v.y;
		vertices[i*4 + 2] = v.z;
		vertices[i*4 + 3] = 1;

		n = tempNormals[indices[i]];
		normals[i*4] = n.x;
		normals[i*4 + 1] = n.y;
		normals[i*4 + 2] = n.z;
		normals[i*4 + 3] = 1;
		
	}

	glBindBuffer(GL_ARRAY_BUFFER, bufVertices);
	glBufferData(GL_ARRAY_BUFFER, 
		0,
		0,
		GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, 
		verticesCount* sizeof(GLfloat)*4,
		vertices,
		GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, bufNormals);
	glBufferData(GL_ARRAY_BUFFER, 
		0,
		0,
		GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, 
		verticesCount* sizeof(GLfloat)*4,
		normals,
		GL_DYNAMIC_DRAW);
}

/*
*
* Proste obracanie wiatru wokol osi OY
*
*/

void Cloth::RotateWind(GLfloat angle){

	wind = realWind;
	glm::detail::tvec3<GLfloat> a(wind.x, wind.y, wind.z);
	a = glm::rotateY(a, angle);
	wind = glm::vec4(a.x, a.y, a.z, 0);
}