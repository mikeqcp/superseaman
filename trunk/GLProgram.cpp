#include "GLProgram.h"
#include "Physics.h"
#include "ModelStuctures.h"

#pragma region Global variables

#define MAXANGLE 60.0f

char windowTitle[255]; //tablica do tytu�u okna
int windowPositionX = 0, windowPositionY = 0; //pozycja okna, kt�re zostanie stworzone
int windowWidth, windowHeight; //szeroko�� i wysoko�� okna
long long frames = 0, time = 0, previousFPSChangeAt = 0, changeFPSAt = 1000; //do wy�witlania FPS�w

glm::mat4 P, V, M; //kolejno macierz projekcji, widoku i modelu
glm::vec4 lightPos; //pozycja �wiat�a

GLfloat angle = 0;
GLfloat sailAngle = 0;

Model *arrow; //strza�ka, kt�ra wskazuje, w kt�r� stron� wieje wiatr
Boat *boat; // to wskazuje na �odk�

GLfloat cameraAngle; //k�t obrotu kamery
glm::vec3 observerPos(0, 5.0f, 7.0f); //Pozycja obserwatora - kamery

GLfloat adder = 1.0f;

GLdouble clipPlane[4] = { 0.0, 1.0, 0.0, 0.0 };
GLuint reflectionTex, refractionTex, depthTex;

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
* boat - ��dka, kt�ra b�dzie renderowana, sk�ada si� z kad�ubu (klasa Model) i �agla (klasa Cloth)
* 
*/

void Initialize(){

	glEnable(GL_DEPTH_TEST);

	V = glm::lookAt(observerPos, glm::vec3(0.0f, 1.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f)); 
	M = glm::mat4(1); 

	lightPos = glm::vec4(10,10,10,1);

	arrow = new Model("Models/arrow.obj", P, V, M, "vshader.txt", "fshader.txt"), 

	boat = new Boat( 
		new Model("Models/boat.obj", P, V, M, "vshader.txt", "fshader.txt"),
		new Cloth("Models/sail.obj", P, V, M, "vshader.txt", "fshader.txt")
		);
	boat->SetWind(glm::vec4(0, 0, 10, 0));
	Physics::instance()->initialize(boat);

	int texCount = 2;
	char *fileNames[] = { 
		"Models/wood.tga", 
		"Models/woodplanks.tga" 
	};										//nazwy plik�w tekstur w postaci tablicy lancuch�w
	SetUpTextures(fileNames, texCount);		//zaladowanie tekstur z plik�w

	boat ->SetTextures(textures, texCount); //wys�anie uchwytow i nazw tekstur do modelu lodzi i zagla

}

/*
*
* Inicjalizacja gry, grupa wywo�a� funkcji inicjalizuj�cych, 
*
*/

void InitializeGame(int argc, char **argv){

	InitOpenGL(45.0f, 800, 600);
	InitGLUT(&argc, argv);
	InitGLEW();
	Initialize();

}

/*
*
* Inicjalizacja wska�nik�w funkcji oraz okna - pozycji, rozmiaru i tytu�u
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

}

/*
*
* ustawienie k�ta pod jakim wida� scen�, oraz wywo�anie funkcji inicjalizuj�cej macierz projekcji
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
* Funkcja wywo�ywana w p�tli kilkadziesi�t razy na sekund�
* S�u�y przekazywaniu obiektom zaktualizowanych danych - macierzy projekcji, widoku i modelu oraz pozycji �wiat�a
* Oblicza te� macierz modelu, przez co ��dka jest obracana
*
*
*/

#include "glm\gtx\rotate_vector.hpp" //TODO do wywalenia p�niej
void Update(){
	
	glm::vec3 basicWind(0, 0, 10);

	//TODO wywalic------------------------------

	M = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, 0, 0)), angle, glm::vec3(0,1,0));

	glm::detail::tvec3<GLfloat> a(basicWind.x, basicWind.y, basicWind.z);
	a = glm::rotateY(a, -angle);
	glm::vec4 wind = glm::vec4(a.x, a.y, a.z, 1);

	boat ->SetWind(wind);

	boat->RotateSail(sailAngle);

	//TEST---------------------------------------

	boat->Update(P, V, M, lightPos);

	//TODO wywalic------------------------------

	M = M*glm::translate(glm::mat4(1), glm::vec3(-3, 0, 0))*glm::rotate(glm::mat4(1), -90.0f, glm::vec3(0, 1, 0))*glm::rotate(glm::mat4(1), -angle, glm::vec3(0, 1, 0));

	//TEST---------------------------------------
	
	arrow -> Update(P, V, M, lightPos);
	angle += 0.5f;
}

/*
*
* Funkcja s�u��ca do obs�ugi klawiatury
* w argumencie c znajduje si� ASCII naci�ni�tego przycisku
*
*/

void KeyboardEvent(unsigned char c, int x, int y){

	if(c == 'a' && sailAngle > -MAXANGLE) sailAngle -= 3;
	if(c == 'd' && sailAngle < MAXANGLE) sailAngle += 3;
	if(c == 27 ) exit(0);

	printf("%d\n", c);

	glutPostRedisplay();

}

/*
*
* Funkcja wywo�ywana kilkadziesi�t razy na minut�
* S�u�y aktualizacji obiekt�w
* Ustawia te� tytu� okna na ilo�� FPS
*
*/

void NextFrame(){

	Update();

	time = glutGet(GLUT_ELAPSED_TIME);

	if(time > changeFPSAt){
		sprintf(windowTitle, "FPS: %f", (GLfloat)frames/(time - changeFPSAt + 1000)*1000); 

		glutSetWindowTitle(windowTitle);
		changeFPSAt = time + 1000;
		frames = 0;
	}

	glutPostRedisplay();

}

void ReshapeWindow(int w, int h){

	glViewport(0, 0, w, h);
	SetupProjection(cameraAngle, w, h);

}

void SetupProjection(GLfloat angle, int w, int h){

	P = glm::perspective(angle, (GLfloat)w/(GLfloat)h, 1.0f, 100.0f);

}

#pragma endregion

#pragma region Drawing

/*
*
* G��wna funkcja rysuj�ca scen�, nie do edycji
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
* Funkcja, w kt�rej wywo�ywane s� metody rysuj�ce obiekty
*
*
*/

void Draw(){

	boat ->Draw();
	arrow ->Draw();
	//RenderReflection();
	//RenderRefractionAndDepth();
	//RenderWater();
	frames++;

}

//woda - narazie zignoruj

void RenderReflection()
{
	glViewport(0,0, windowWidth, windowHeight);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec4 plane(0, 1, 0, 0);
	double p[] = {0, 1, 0, 0};
	boat->SetClipPlane(plane);
   	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, p);
   	boat ->DrawReflection();
   	glDisable(GL_CLIP_PLANE0);

   	glBindTexture(GL_TEXTURE_2D, reflectionTex);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,0, 0,  windowWidth, windowHeight, 0); 

	//TODO sprawdzi�, czy to rzeczywi�cie robi screena
   	
}

void RenderRefractionAndDepth(){

   	glViewport(0,0, windowWidth, windowHeight);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   	double plane[4] = {0.0, -1.0, 0.0, 0.0}; 
   	glEnable(GL_CLIP_PLANE0);
   	glClipPlane(GL_CLIP_PLANE0, plane);
   	boat ->Draw();
   	glDisable(GL_CLIP_PLANE0);

   	glBindTexture(GL_TEXTURE_2D, refractionTex);
   	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0, windowWidth, windowHeight);

   	glBindTexture(GL_TEXTURE_2D, depthTex);
   	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0,0, windowWidth, windowHeight, 0);

}

void RenderWater()
{

    //bind all textures
   	glActiveTextureARB(GL_TEXTURE0_ARB);
   	glEnable(GL_TEXTURE_2D);	
   	glBindTexture(GL_TEXTURE_2D, reflectionTex);

   //	glActiveTextureARB(GL_TEXTURE1_ARB);
   //	glEnable(GL_TEXTURE_2D);
   //	glBindTexture(GL_TEXTURE_2D, refractionTex);

   	//glActiveTextureARB(GL_TEXTURE2_ARB);
   	//glEnable(GL_TEXTURE_2D);	
   	//glBindTexture(GL_TEXTURE_2D, normalmap);

   	//glActiveTextureARB(GL_TEXTURE3_ARB);
   	//glEnable(GL_TEXTURE_2D);
   	//glBindTexture(GL_TEXTURE_2D, dudvmap);

   	glActiveTextureARB(GL_TEXTURE4_ARB);
   	glEnable(GL_TEXTURE_2D);	
   	glBindTexture(GL_TEXTURE_2D, depthTex);

    //Render the water surface 
   	glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 5.0f);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 1.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 1.0f);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 5.0f, 0.0f);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 1.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 5.0f, 5.0f);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 1.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glEnd();

    //Disable texture units and shader programs
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

#pragma endregion
