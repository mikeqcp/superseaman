#pragma once

#include "Vector3D.h"
#include <math.h>

#define PI 3.14159265

template<class T = int, int x = 4, int y = 4> 
class MatrixBase
{

private:

	T **matrix;

public:

	MatrixBase();

	MatrixBase(T **matrix);
	
	~MatrixBase();

	int getX(); int getY();

	void createRotation(Vector3D vector, GLfloat theta);

	void createRotation(Vector3D vector, int theta);
};

template<class T, int x, int y>
MatrixBase<T, x, y>::MatrixBase()
{
	matrix = new T*[x];
	for(int i = 0; i < x; i++)
		matrix[i] = new T[y];
}

template<class T, int x, int y> 
MatrixBase<T, x, y>::MatrixBase(T **matrix) : MatrixBase<T, x, y>()
{
	for(int i = 0; i < x; i++)
		for(int j = 0; j < y; j++)
			this -> matrix[i][j] = matrix[i][j];
}

template<class T, int x, int y> 
MatrixBase<T, x, y>::~MatrixBase()
{
}

template<class T, int x, int y> 
int MatrixBase<T, x, y>::getX()
{
	return x;
}

template<class T, int x, int y> 
int MatrixBase<T, x, y>::getY()
{
	return y;
}

template<class T, int x, int y> 
void MatrixBase<T, x, y>::createRotation(Vector3D vector, GLfloat theta)
{
	if(x != 4 || y != 4){
		throw exception;
		return;
	}

	//TODO uzupe³niæ macierz rotacji

}

template<class T, int x, int y> 
void MatrixBase<T, x, y>::createRotation(Vector3D vector, int theta)
{
	if(x != 4 || y != 4){
		throw exception;
		return;
	}

	createRotation(vector, theta*PI/180.0f);

}