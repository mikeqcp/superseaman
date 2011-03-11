// NeHe_01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GLWindow.h"

int _tmain(int argc, _TCHAR* argv[])
{
	GLWindow glWindow(L"Dupa", 1024, 768, 32, false);
	glWindow.StartLoop();

	return 0;
}