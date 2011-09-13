#include "GLProgram.h"
#include "Physics.h"
#include "ModelStuctures.h"
#include "SailingObject.h"
#include "Camera.h"
#include <math.h>

#pragma region Global variables

#define MAXANGLE 60.0f



char windowTitle[255]; //tablica do tytu³u okna
int windowPositionX = 0, windowPositionY = 0; //pozycja okna, które zostanie stworzone
int windowWidth, windowHeight; //szerokoœæ i wysokoœæ okna
long long frames = 0, time = 0, previousFPSChangeAt = 0, changeFPSAt = 1000; //do wyœwitlania FPSów

glm::mat4 P, V, M; //kolejno macierz projekcji, widoku i modelu
glm::mat4 Mboat, Mwind;
glm::vec4 lightPos; //pozycja œwiat³a

Camera *camera;	//kamera

GLfloat angle = 0;
GLfloat sailAngle = 0;

Model *arrow; //strza³ka, która wskazuje, w któr¹ stronê wieje wiatr
Boat *boat; // to wskazuje na ³odkê
SailingObject *boatPhysics;	//wskazuje na model fizyczny ³ódki (zostawiam jako osobna zmienna bo czesto sie do tego bedziemy odwo³ywaæ)
Water *water;
Model *skyDome;
Model *terrain;


GLfloat cameraAngle; //k¹t obrotu kamery
glm::vec3 observerPos(-15.0f, 10.0f, 0.0f); //Pozycja obserwatora - kamery
glm::vec3 lookAtPos(0, 1, 0); //na co patrzy kamera

GLint steer = 0;	//-1 ster w lewo, 1 w prawo, 0 luz 
GLfloat adder = 1.0f;

GLdouble clipPlane[4] = { 0.0, 1.0, 0.0, 0.0 };
GLuint reflectionTex, refractionTex, depthTex;
GLuint reflectionFBO, refractionAndDepthFBO;

Texture *textures;

#pragma endregion

#pragma region initialization - segment inicjalizujacy

/*
*
* Inicjalizacja zmiennych, 
*
* M - macierz modelu
* V - macierz widoku
* 
* boat - ³ódka, która bêdzie renderowana, sk³ada siê z kad³ubu (klasa Model) i ¿agla (klasa Cloth)
* 
*/

void Initialize(){

	glEnable(GL_DEPTH_TEST);

	V = glm::lookAt(observerPos, lookAtPos, glm::vec3(0.0f,1.0f,0.0f)); 
	M = glm::mat4(1); 

	lightPos = glm::vec4(10,10,10,1);

	arrow = new Model("Models/arrow.obj", P, V, M, "Shaders/arrowvshader.txt", "Shaders/arrowfshader.txt");
	water = new Water("Models/waterPlane.obj", P, V, M, "Shaders/watervshader.txt", "Shaders/waterfshader.txt");
	skyDome = new Model("Models/skyDome.obj", P, V, M, "Shaders/skyvshader.txt", "Shaders/skyfshader.txt");
	terrain = new Model("Models/terrain.obj", P, V, M, "Shaders/vshader.txt", "Shaders/fshader.txt");

	boat = new Boat( 
		new Model("Models/boat.obj", P, V, M, "Shaders/vshader.txt", "Shaders/fshader.txt"),
		new Cloth("Models/sail.obj", P, V, M, "Shaders/vshader.txt", "Shaders/fshader.txt")
		);
	boat->SetWind(glm::vec4(0, 0, 10, 0));

	int texCount = 7;
	char *fileNames[] = { 
		"Models/wood.tga", 
		"Models/woodplanks.tga",
		"Models/fabric.tga",
		"Models/SkyDome-Cloud-Medium-MidMorning.tga",
		"Models/normal.tga",
		"Models/ao.tga",
		"Models/terrain.tga"
	};										//nazwy plików tekstur w postaci tablicy lancuchów
	SetUpTextures(fileNames, texCount);		//zaladowanie tekstur z plików

	boat ->SetTextures(textures, texCount); //wys³anie uchwytow i nazw tekstur do modelu lodzi i zagla
	skyDome ->SetTextures(textures, texCount);
	terrain ->SetTextures(textures, texCount);

	SetupFBO(reflectionFBO, reflectionTex);
	SetupFBO(refractionAndDepthFBO, refractionTex);
	CreateDepthTex();

	water ->SetNormalAndDUDVMapsTex(LoadTexture("Models/Water2/waterNormalMap.tga"), LoadTexture("Models/Water2/waterDUDVMap.tga"));

	//inicjalizacja fizyki
	//PhysicalObject *boat = new PhysicalObject(1.0, glm::vec4(0,-1,0,0));
	boatPhysics = new SailingObject(1.0, glm::vec4(0,-1,0,0));
	PhysicalObject **allObjects = new PhysicalObject *[1];
		allObjects[0] = boatPhysics;

	Physics::instance()->setTargets(allObjects, 1);
	camera = new Camera(boatPhysics, V);

}

/*
*
* Inicjalizacja gry, grupa wywo³añ funkcji inicjalizuj¹cych, 
*
*/

void InitializeGame(int argc, char **argv){

	InitOpenGL(45.0f, 500, 500);
	InitGLUT(&argc, argv);
	InitGLEW();
	Initialize();

}

/*
*
* Inicjalizacja wskaŸników funkcji oraz okna - pozycji, rozmiaru i tytu³u
* rozmiar ze zmiennych globalnych widnowPositionX i Y
* 
*
*/

void InitGLUT(int *argc, char **argv){

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(windowPositionX, windowPositionY);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Super Sailor 2011");

	glutReshapeFunc(ReshapeWindow);
	glutDisplayFunc(DisplayFrame);
	glutIdleFunc(NextFrame);
	glutKeyboardFunc(KeyboardEvent);
	glutKeyboardUpFunc(KeyUpEvent);
	//glutSpecialFunc(KeyboardEvent);

}

/*
*
* ustawienie k¹ta pod jakim widaæ scenê, oraz wywo³anie funkcji inicjalizuj¹cej macierz projekcji
*
*
*/

void InitOpenGL(GLfloat angle, int w, int h){

	cameraAngle = angle;
	windowWidth = w;
	windowHeight = h;

	SetupProjection(angle, w, h);

}


void InitGLEW(){

	GLenum err = glewInit();
	if (GLEW_OK != err) {

		fprintf(stderr,"%s\n",glewGetErrorString(err));
		exit(1);

	}
	
	if (!GLEW_VERSION_3_0) {

		printf("OpenGL 3.0 NOT supported\n");
		getchar();
		exit(0);

	}

}

#pragma endregion

#pragma region Update

/*
*
*
* Funkcja wywo³ywana w pêtli kilkadziesi¹t razy na sekundê
* S³u¿y przekazywaniu obiektom zaktualizowanych danych - macierzy projekcji, widoku i modelu oraz pozycji œwiat³a
* Oblicza te¿ macierz modelu, przez co ³ódka jest obracana
*
*
*/

#include "glm\gtx\rotate_vector.hpp" //TODO do wywalenia póŸniej
void Update(){

	//V = glm::lookAt(observerPos, lookAtPos, glm::vec3(0.0f,1.0f,0.0f));
	V = camera->update();

	terrain ->Update(P, V, glm::rotate(glm::translate(glm::mat4(1), glm::vec3(9, 0.25f, 0)), -90.0f, glm::vec3(0, 1, 0)), lightPos);

	//TODO wywalic------------------------------

	//M = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, 0, 0)), angle, glm::vec3(0,1,0));
	M = glm::mat4(1);

	//glm::detail::tvec3<GLfloat> a(basicWind.x, basicWind.y, basicWind.z);
	//a = glm::rotateY(a, -angle);
	//glm::vec4 wind = glm::vec4(a.x, a.y, a.z, 1);

	//boat ->SetWind(wind);

	Physics::instance()->update();

	boat->RotateSail(sailAngle);
	//dynamic_cast<SailingObject*>(Physics::instance()->getTargets()[0])->setClothAngle(sailAngle);
	boatPhysics->setClothAngle(sailAngle);

	//TEST---------------------------------------

	//M = Physics::instance()->getStates()[0].translation ;
	Mboat = Physics::instance()->getStates()[0].translation * Physics::instance()->getStates()[0].rotation;
	
	Mwind = glm::rotate(glm::mat4(1), boatPhysics->getDirAngle(), glm::vec3(0,1,0));
	glm::vec4 windTemp = Physics::instance()->getWind();
	boat->SetWind(glm::inverse(Mwind) * windTemp);

	boat->Update(P, V, Mboat, lightPos);
	boat->SetLookAtPos(lookAtPos);
	
	Mwind = glm::mat4(1);
	Mwind[3] = Mboat[3];
	arrow -> Update(P, V, glm::translate(glm::mat4(1), glm::vec3(-1, 1, 0)) * Mwind * glm::rotate(glm::mat4(1), -90.0f, glm::vec3(0, 1, 0)) * Physics::instance()->getWindScaleMatrix() * Physics::instance()->getWindMatrix(), lightPos);
	arrow ->SetLookAt(lookAtPos);

	//water ->Update(P, V, glm::rotate(glm::mat4(1), 90.0f, glm::vec3(0, 1, 0)), lightPos);
	water ->Update(P, V, Mwind * glm::rotate(glm::mat4(1), 90.0f, glm::vec3(0, 1, 0)), lightPos);
	water ->SetLookAt(lookAtPos);
	water ->SetViewPos(observerPos);

	skyDome ->Update(P, V, Mwind, lightPos);
}

/*
*
* Funkcja s³u¿¹ca do obs³ugi klawiatury
* w argumencie c znajduje siê ASCII naciœniêtego przycisku
*
*/

void KeyboardEvent(unsigned char c, int x, int y){

	if(c == 'a' && sailAngle > -MAXANGLE) sailAngle -= 3;
	if(c == 'd' && sailAngle < MAXANGLE) sailAngle += 3;
	if(c == 'w'){
		lookAtPos += glm::vec3(0.05f, 0, 0);
		observerPos += glm::vec3(0.05f, 0, 0);
	}
	if(c == 's'){
		lookAtPos += glm::vec3(-0.05f, 0, 0);
		observerPos += glm::vec3(-0.05f, 0, 0);
	}
	if(c == 'd'){
		lookAtPos += glm::vec3(0, 0, 0.05f);
		observerPos += glm::vec3(0, 0, 0.05f);
	}
	if(c == 'a'){
		lookAtPos += glm::vec3(0, 0, -0.05f);
		observerPos += glm::vec3(0, 0, -0.05f);
	}

	if(c == 'i'){
		lookAtPos += glm::vec3(0.05f, 0, 0);
	}
	if(c == 'k'){
		lookAtPos += glm::vec3(-0.05f, 0, 0);
	}
	if(c == 'j'){
		lookAtPos += glm::vec3(0, 0, 0.05f);
	}
	if(c == 'l'){
		lookAtPos += glm::vec3(0, 0, -0.05f);
	}


	if(c == 'b'){
		boatPhysics->setSteer(-1);
	}
	if(c == 'm'){
		boatPhysics->setSteer(1);
	}
	
	if(c == 27 ) exit(0);

	//printf("%d\n", c);

	glutPostRedisplay();

}

void KeyUpEvent(unsigned char c, int x, int y)
{
	if(c == 'b' || c == 'm')
	{
		boatPhysics->setSteer(0);
	}

}


/*
*
* Funkcja wywo³ywana kilkadziesi¹t razy na minutê
* S³u¿y aktualizacji obiektów
* Ustawia te¿ tytu³ okna na iloœæ FPS
*
*/

void NextFrame(){

	Update();

	time = glutGet(GLUT_ELAPSED_TIME);

	if(time > changeFPSAt){
		sprintf_s(windowTitle, "FPS: %f", (GLfloat)frames/(time - changeFPSAt + 1000)*1000); 

		glutSetWindowTitle(windowTitle);
		changeFPSAt = time + 1000;
		frames = 0;
	}

	glutPostRedisplay();

}

void ReshapeWindow(int w, int h){

	glViewport(0, 0, w, h);
	windowWidth = w;
	windowHeight = h;
	glDeleteFramebuffersEXT(1, &reflectionFBO);
	glDeleteTextures(1, &reflectionTex);
	SetupFBO(reflectionFBO, reflectionTex);
	SetupFBO(refractionAndDepthFBO, refractionTex);
	CreateDepthTex();
	SetupProjection(cameraAngle, w, h);

}

void SetupProjection(GLfloat angle, int w, int h){

	P = glm::perspective(angle, (GLfloat)w/(GLfloat)h, 1.0f, 100.0f);

}

#pragma endregion

#pragma region Drawing

/*
*
* G³ówna funkcja rysuj¹ca scenê, nie do edycji
*
*/

void DisplayFrame(){

	glClearColor(0.3921f, 0.5843f, 0.9294f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw();

	glutSwapBuffers();

}

/*
*
* Funkcja, w której wywo³ywane s¹ metody rysuj¹ce obiekty
*
*
*/

void Draw(){

	RenderReflection();
	RenderRefractionAndDepth();

	terrain ->Draw();
	water ->Draw();
	skyDome ->Draw();
	boat ->Draw();
	arrow ->Draw();
	frames++;

}

//woda - narazie zignoruj

void RenderReflection()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, reflectionFBO);

	glViewport(0,0, windowWidth, windowHeight);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec4 plane(0, 1, 0, 0);
	double p[] = {0, 1, 0, 0};
	boat->Update(P, V, glm::inverse(boatPhysics->getCurrentPosition().rotation), lightPos);
	boat->SetClipPlane(plane);
	
   	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, p);

   	boat ->DrawReflection();
	skyDome ->DrawReflection();
	terrain ->DrawReflection();
   	glDisable(GL_CLIP_PLANE0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	glBindTexture(GL_TEXTURE_2D, reflectionTex);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	water ->SetReflectionTex(reflectionTex);
   	boat->Update(P, V, boatPhysics->getCurrentPosition().rotation, lightPos);
}

void RenderRefractionAndDepth(){

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, refractionAndDepthFBO);

	glViewport(0,0, windowWidth, windowHeight);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec4 plane(0, -1, 0, 0);
	double p[] = {0, -1, 0, 0};
	boat->SetClipPlane(plane);
	terrain ->SetClipPlane(plane);
   	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, p);

   	boat ->Draw();
	skyDome ->Draw();
	terrain ->Draw();
   	glDisable(GL_CLIP_PLANE0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	glBindTexture(GL_TEXTURE_2D, refractionTex);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	boat ->Draw();
	terrain ->Draw();

	glBindTexture(GL_TEXTURE_2D, depthTex);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, windowWidth, windowHeight);
    glBindTexture(GL_TEXTURE_2D, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	water ->SetRefractionAndDepthTex(refractionTex, depthTex);

}
#pragma endregion

#pragma region Additional Functions

GLuint LoadTexture(char* filename){

	GLuint tex = 0;
	TGAImg img;
	glActiveTexture(GL_TEXTURE0);

	if (img.Load(filename) == IMG_OK) {

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		if (img.GetBPP()==24)
			glTexImage2D(GL_TEXTURE_2D, 0, 3, img.GetWidth(), img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.GetImg());
		else if (img.GetBPP()==32) 
			glTexImage2D(GL_TEXTURE_2D, 0, 4, img.GetWidth(), img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.GetImg());
		else printf("Nieobslugiwany format obrazka w pliku: %s \n",filename);

	} else printf("Blad przy wczytywaniu pliku: %s\n",filename);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return tex;

}

void SetUpTextures(char **fileNames, int texCount){

	textures = new Texture[texCount];
	string fileName;
	for(int i = 0; i < texCount; i++){

		fileName = string(fileNames[i]);
		int pos1 = fileName.find_last_of('/')+1;
		int pos2 = fileName.find_last_of('.');
		textures[i].name = fileName.substr(pos1, pos2-pos1);
		textures[i].tex = LoadTexture(fileNames[i]);

	}

}

void SetupFBO(GLuint &fbo, GLuint &tex){

	if(tex) glDeleteTextures(1, &tex);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint rboId;
	glGenRenderbuffersEXT(1, &rboId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	if(fbo) glDeleteFramebuffers(1, &fbo);
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rboId);

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

void CreateDepthTex(){

	if(depthTex) glDeleteTextures(1, &depthTex);
	glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

#pragma endregion
