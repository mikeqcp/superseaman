#pragma once
#include "includes.h"
#include "Cloth.h"
#include "Model.h"
#include "Boat.h"

using namespace std;

void InitGLUT(int *argc, char **argv);
void InitOpenGL(GLfloat angle, int w, int h);
void InitGLEW();
void InitializeGame(int argc, char **argv);
void Initialize();

void ReshapeWindow(int w, int h);
void SetupProjection(GLfloat angle, int w, int h);
void DisplayFrame();
void NextFrame();
void KeyboardEvent(unsigned char c, int x, int y);

void Draw();
void Update();

GLuint LoadShader(GLenum shaderType, string fileName);

string ReadFile(string fileName);
