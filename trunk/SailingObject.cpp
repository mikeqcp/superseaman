#include "SailingObject.h"
#include "Physics.h"
#include <math.h>


SailingObject::~SailingObject(void)
{
}

SailingObject::SailingObject(GLfloat mass, glm::vec4 cenOfGrav) : PhysicalObject(mass, cenOfGrav)
{
	axisAngle = 0;
	dirAngle = 0;
	sailAngle = 0;
	prevAxisAngle = 0;
	steer =0;

	prevM = glm::mat4(1);
}

Result SailingObject::update()
{
	Result res;

	Result resWind = updateWind();
	Result resVertical = updateVertical();
	

	//res.translation = resWind.translation ;
	res.translation = resWind.translation * resVertical.translation;
	res.rotation = resWind.rotation;

	return res;
}

GLfloat SailingObject::getAngle(glm::vec4 *vec)
{
	GLfloat angle= 180/ PI *atan(vec->x/vec->z);

	if(vec->x>0)
	{
		if(vec->z>0)
			{}
		else
			angle = 180 + angle;
	}
	else
	{
		if(vec->z>0)
			angle = 360 + angle;
		else
			angle = angle + 180;
	}

	return angle +270;
}

Result SailingObject::updateWind()
{
	Result res;
	GLfloat force = 0;	//si³a noœna ¿agla
	GLfloat forwardForce;	//si³a ci¹gu ¿agla
	GLfloat sideForce;	//si³a przechy³u
	GLfloat angle;	//k¹t przechy³u
	GLfloat reactionForce;	//si³a prostuj¹ca

	glm::vec4 wind = Physics::instance()->getWind();

	GLfloat windAngle = fmod(getAngle(&wind) - dirAngle + sailAngle, 360);
	
	force = wind.w * sin(degToRad(windAngle));
	//przechy³
	sideForce = 0.0003 * (force * cos(degToRad(sailAngle)));
	reactionForce = -0.01*cos(degToRad(axisAngle-90));
	

	angle = axisAngle + axisAngle - prevAxisAngle + sideForce + reactionForce - 0.01 * steer;

		if(angle > 30 )	angle = 30;
			else if(angle <-30 ) angle = -30;

	res.rotation = glm::rotate(glm::mat4(1), -angle, glm::vec3(1, 0, 0));

	prevAxisAngle = axisAngle;
	axisAngle = angle;

	//ruch
	forwardForce = 0.0003 * (force * sin(degToRad(sailAngle)));
	res.translation = updateMovement(forwardForce).translation;
	//res.rotation = updateMovement(forwardForce).rotation;

	return res;
}

Result SailingObject::updateMovement(GLfloat force)
{
	Result res;
	glm::mat4 newM = glm::mat4(1);

	glm::mat4 temp = M * glm::inverse(prevM);

	/*glm::vec3 diff = -glm::vec3(prevM[3].x, prevM[3].y, prevM[3].z) + glm::vec3(M[3].x, M[3].y, M[3].z);
	glm::mat4 temp2 = glm::mat4(1);
		temp2[3].x = diff.x;
		temp2[3].y = diff.y;
		temp2[3].z = diff.z;
		temp = temp2;*/

	GLfloat steerAngle = steer * 0.1;
	//dirAngle += steer * 0.1;

	newM = M * glm::rotate(glm::mat4(1), steerAngle, glm::vec3(0,1,0)) * glm::translate(glm::mat4(1), glm::vec3(0.01,0,0)) * glm::inverse(M);

	prevM = M;
	M = newM * M;	//w M trzymam tylko po³o¿enie poziome (tj miejsce po³ozenia i skrêt wokó³ osi pionowej)
	res.translation =  M;

	
	//res.rotation = ;

	return res;
}
