#pragma once
#include "includes.h"

class Material
{
private:
	string name;
	GLfloat Ns, Ni, d;
	glm::vec4 Ka, Kd, Ks;
	int illum;
	string map_Kd, map_Ka, map_Ks, map_Kn;

public:

	Material(void);

	~Material(void);

	void setName(string name){ this -> name = name; }

	void setNs(GLfloat Ns){ this -> Ns = Ns; }

	void setNi(GLfloat Ni){ this -> Ni = Ni; }

	void setD(GLfloat d){ this -> d = d; }

	void setKa(glm::vec4 Ka){ this -> Ka = Ka; }

	void setKd(glm::vec4 Kd){ this -> Kd = Kd; }

	void setKs(glm::vec4 Ks){ this -> Ks = Ks; }

	void setIllum(int illum){ this -> illum = illum; }

	void setMap_Kd(string map_Kd){ this -> map_Kd = map_Kd; }

	void setMap_Ka(string map_Ka){ this -> map_Ka = map_Ka; }

	void setMap_Ks(string map_Ks){ this -> map_Ks = map_Ks; }

	void setMap_Kn(string map_Kn){ this -> map_Kn = map_Kn; }

	string getName(){ return name; }

	GLfloat getNs(){ return Ns; }

	GLfloat getNi(){ return Ni; }

	GLfloat getD(){ return d; }

	glm::vec4 getKa(){ return Ka; }

	glm::vec4 getKd(){ return Kd; }

	glm::vec4 getKs(){ return Ks; }

	int getIllum(){ return illum; }

	string getMap_Kd(){ return map_Kd; }

	string getMap_Ka(){ return map_Ka; }

	string getMap_Ks(){ return map_Ks; }

	string getMap_Kn(){ return map_Kn; }
};

