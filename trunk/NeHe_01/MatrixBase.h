#pragma once

#include "Vector3D.h"
#include <math.h>

#define PI 3.14159265

class MatrixBase
{

private:

	GLfloat **matrix;
	int x, y;

public:

	explicit MatrixBase(int x = 4, int y = 4);

	MatrixBase(GLfloat **matrix, int x = 4, int y = 4);
	
	~MatrixBase();

	int getX(); int getY();

	void set(GLfloat val, int x, int y){ matrix[x][y] = val; }

	GLfloat get(int x, int y){ return matrix[x][y]; }

	void createRotation(Vector3D vector, GLfloat theta);

	void createRotation(Vector3D vector, int theta);

	MatrixBase &operator*(MatrixBase &mb);

	MatrixBase &operator=(MatrixBase &mb);
};


