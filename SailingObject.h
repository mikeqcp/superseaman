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
	Result updateWind();	//uwzglêdnia si³y w poziomie (wiatr), zwraca przesuniêcie

	void setClothAngle(GLfloat angle) {sailAngle = -angle;};
	void setSteer(GLint val) {steer= val;};

private:

	GLfloat dirAngle;	//k¹t kierunku
	GLfloat sailAngle;	//k¹t ustawienia ¿agla

	GLfloat prevAxisAngle;	//potrzebne do Verletta

	GLfloat getAngle(glm::vec4 *vec);

	glm::mat4 prevM;	//poprzednie po³o¿enie

	Result updateMovement(GLfloat force);
	GLint steer;
};

