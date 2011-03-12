#pragma once
class Material
{
private:
	string name;
	GLfloat Ns, Ni, d;
	Vertex Ka, Kd, Ks;
	int illum;
	string map_Kd;

public:

	Material(void);

	~Material(void);

	void setName(string name){ this -> name = name; }

	void setNs(GLfloat Ns){ this -> Ns = Ns; }

	void setNi(GLfloat Ni){ this -> Ni = Ni; }

	void setD(GLfloat d){ this -> d = d; }

	void setKa(Vertex Ka){ this -> Ka = Ka; }

	void setKd(Vertex Kd){ this -> Kd = Kd; }

	void setKs(Vertex Ks){ this -> Ks = Ks; }

	void setIllum(int illum){ this -> illum = illum; }

	void setMap_Kd(string map_Kd){ this -> map_Kd = map_Kd; }

	string getName(){ return name; }

	GLfloat getNs(){ return Ns; }

	GLfloat getNi(){ return Ni; }

	GLfloat getD(){ return d; }

	Vertex getKa(){ return Ka; }

	Vertex getKd(){ return Kd; }

	Vertex getKs(){ return Ks; }

	int getIllum(){ return illum; }

	string getMap_Kd(){ return map_Kd; }
};

