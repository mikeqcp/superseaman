#pragma once
#include "includes.h"
#include "Cloth.h"
#include "Model.h"
#include "Boat.h"
#include "tga.h"

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

void RenderReflection();
void RenderRefractionAndDepth();
void RenderWater();

GLuint LoadTexture(char* fileName);
void SetUpTextures(char **fileNames, int texCount);

string ReadFile(string fileName);
