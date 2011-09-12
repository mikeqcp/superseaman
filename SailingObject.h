#pragma once
#include "physicalobject.h"
class SailingObject :
	public PhysicalObject
{
public:

	~SailingObject(void);

	SailingObject(GLfloat mass) : PhysicalObject(mass) {};
	SailingObject(GLfloat mass, glm::vec4 cenOfGrav);

	Result update();	
	Result updateWind();	//uwzgl�dnia si�y w poziomie (wiatr), zwraca przesuni�cie

	void setClothAngle(GLfloat angle) {sailAngle = -angle;};
	void setSteer(GLint val) {steer= val;};

private:

	GLfloat dirAngle;	//k�t kierunku
	GLfloat sailAngle;	//k�t ustawienia �agla

	GLfloat prevAxisAngle;	//potrzebne do Verletta

	GLfloat getAngle(glm::vec4 *vec);

	glm::mat4 prevM;	//poprzednie po�o�enie

	Result updateMovement(GLfloat force);
	GLint steer;
};

