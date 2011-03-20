#include "StdAfx.h"
#include "MatrixBase.h"


MatrixBase::MatrixBase(int x, int y)
{
	this ->x = x;
	this ->y = y;
	matrix = new GLfloat*[x];
	for(int i = 0; i < x; i++)
		matrix[i] = new GLfloat[y];
}


MatrixBase::MatrixBase(GLfloat **mat, int x, int y) 
{
	this ->x = x;
	this ->y = y;
	matrix = new GLfloat*[x];
	for(int i = 0; i < x; i++)
		matrix[i] = new GLfloat[y];

	for(int i = 0; i < x; i++)
		for(int j = 0; j < y; j++)
			this -> matrix[i][j] = mat[i][j];
}


MatrixBase::~MatrixBase()
{
}

int MatrixBase::getX()
{
	return x;
}


int MatrixBase::getY()
{
	return y;
}


void MatrixBase::createRotation(Vector3D vector, GLfloat theta)
{
	if(x != 4 || y != 4)
		throw "WrongSize";
	
	GLfloat c = cos(theta);
	GLfloat s = sin(theta);
	GLfloat t = 1 - c;

	matrix[0][0] = t * vector.x * vector.x + c;
	matrix[0][1] = t * vector.x * vector.y - s * vector.z;
	matrix[0][2] = t * vector.x * vector.z + s * vector.y;
	matrix[0][3] = 0;

	matrix[1][0] = t * vector.x * vector.y + s * vector.z;
	matrix[1][1] = t * vector.y * vector.y + c;
	matrix[1][2] = t * vector.y * vector.z - s * vector.x;
	matrix[1][3] = 0;

	matrix[2][0] = t * vector.x * vector.z - s * vector.y;
	matrix[2][1] = t * vector.y * vector.z + s * vector.x;
	matrix[2][2] = t * vector.z * vector.z + c;
	matrix[2][3] = 0;

	matrix[3][0] = 0;
	matrix[3][1] = 0;
	matrix[3][2] = 0;
	matrix[3][3] = 1;

}


void MatrixBase::createRotation(Vector3D vector, int theta)
{
	if(x != 4 || y != 4)
		throw "WrongSize";
	
	createRotation(vector, (GLfloat)(theta*PI/180.0f));

}


MatrixBase &MatrixBase::operator*(MatrixBase &mb)
{
	int m = x;
	int n = y;
	int p = mb.getY();

	if(n != mb.getX()) 
		throw "WrongSize";

	MatrixBase result(m, p);

	for(int i = 0; i < m; i++){
	
		for(int j = 0; j < p; j++){
		
			GLfloat s = 0;
			for(int k = 0; k < n; k++)
				s += matrix[i][k] * mb.get(k, j);

			result.set(s, i, j);

		}
	}

	return result;

}

MatrixBase &MatrixBase::operator=(MatrixBase &mb){

	this ->x = mb.getX();
	this ->y = mb.getY();
	matrix = new GLfloat*[x];
	for(int i = 0; i < x; i++)
		matrix[i] = new GLfloat[y];

	for(int i = 0; i < x; i++)
		for(int j = 0; j < y; j++)
			this -> matrix[i][j] = mb.get(i, j);

	return *this;

}