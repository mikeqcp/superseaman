#include "GLProgram.h"
#include "Physics.h"

#pragma region Global variables

#define MAXANGLE 60.0f

char windowTitle[255]; //tablica do tytu³u okna
int windowPositionX = 0, windowPositionY = 0; //pozycja okna, które zostanie stworzone
int windowWidth, windowHeight; //szerokoœæ i wysokoœæ okna
long long frames = 0, time = 0, previousFPSChangeAt = 0, changeFPSAt = 1000; //do wyœwitlania FPSów

glm::mat4 P, V, M; //kolejno macierz projekcji, widoku i modelu
glm::vec4 lightPos; //pozycja œwiat³a

GLfloat angle = 0;
GLfloat sailAngle = 0;


Boat *boat; // to wskazuje na ³odkê

GLfloat cameraAngle; //k¹t obrotu kamery
glm::vec3 observerPos(0, 5.0f, 7.0f); //Pozycja obserwatora - kamery

GLfloat adder = 1.0f;

GLdouble clipPlane[4] = { 0.0, 1.0, 0.0, 0.0 };
GLuint reflectionTex, refractionTex, depthTex;

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

	V = glm::lookAt(observerPos, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f)); 
	M = glm::mat4(1); 

	lightPos = glm::vec4(10,10,10,1);

	boat = new Boat( 
		new Model("Models/boat.obj", P, V, M, "vshader.txt", "fshader.txt"),
		new Cloth("Models/sail.obj", P, V, M, "vshader.txt", "fshader.txt")
		);

	Physics::instance()->initialize(boat);
}

/*
*
* Inicjalizacja gry, grupa wywo³añ funkcji inicjalizuj¹cych, 
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

void Update(){

	M = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, -1, 0)), angle, glm::vec3(0,1,0));
	//M = glm::mat4(1);

	boat->RotateSail(sailAngle);
	boat->Update(P, V, M, lightPos);
	
	angle += 0.5f;
	//sailAngle += adder;
	//if(sailAngle > 60.0f || sailAngle < -60.0f)
		//adder *= -1;

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
	if(c == 27 ) exit(0);

	printf("%d\n", c);

	glutPostRedisplay();

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

	boat ->Draw();
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

	//TODO sprawdziæ, czy to rzeczywiœcie robi screena
   	
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
