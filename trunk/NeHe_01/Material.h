#pragma once
#include "Vector3D.h"

class Material
{
private:
	string name;
	GLfloat Ns, Ni, d;
	Vector3D Ka, Kd, Ks;
	int illum;
	string map_Kd;

public:

	Material(void);

	~Material(void);

	void setName(string name){ this -> name = name; }

	void setNs(GLfloat Ns){ this -> Ns = Ns; }

	void setNi(GLfloat Ni){ this -> Ni = Ni; }

	void setD(GLfloat d){ this -> d = d; }

	void setKa(Vector3D Ka){ this -> Ka = Ka; }

	void setKd(Vector3D Kd){ this -> Kd = Kd; }

	void setKs(Vector3D Ks){ this -> Ks = Ks; }

	void setIllum(int illum){ this -> illum = illum; }

	void setMap_Kd(string map_Kd){ this -> map_Kd = map_Kd; }

	string getName(){ return name; }

	GLfloat getNs(){ return Ns; }

	GLfloat getNi(){ return Ni; }

	GLfloat getD(){ return d; }

	Vector3D getKa(){ return Ka; }

	Vector3D getKd(){ return Kd; }

	Vector3D getKs(){ return Ks; }

	int getIllum(){ return illum; }

	string getMap_Kd(){ return map_Kd; }
};

