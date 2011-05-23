#pragma once
#include "includes.h"

class Material
{
private:
	string name;
	GLfloat Ns, Ni, d;
	glm::vec4 Ka, Kd, Ks;
	int illum;
	string map_Kd;

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

	string getName(){ return name; }

	GLfloat getNs(){ return Ns; }

	GLfloat getNi(){ return Ni; }

	GLfloat getD(){ return d; }

	glm::vec4 getKa(){ return Ka; }

	glm::vec4 getKd(){ return Kd; }

	glm::vec4 getKs(){ return Ks; }

	int getIllum(){ return illum; }

	string getMap_Kd(){ return map_Kd; }
};

