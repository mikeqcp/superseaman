#include "GLProgram.h"

#pragma region Global variables

char windowTitle[255];
int windowPositionX = 200, windowPositionY = 200;
int windowWidth, windowHeight;
long long frames = 0, time = 0, previousFPSChangeAt = 0, changeFPSAt = 1000;

glm::mat4 P, V, M;
glm::vec4 lightPos;
GLfloat cameraAngle;
GLfloat angle = 0;
GLfloat sailAngle = 0;
Boat *boat;

glm::vec4 wind(0, 0, 10, 0);

GLfloat adder = 1.0f;

#pragma endregion

#pragma region initialization

void Initialize(){

	glEnable(GL_DEPTH_TEST);

	V = glm::lookAt(glm::vec3(0, 5.0f, 7.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f)); 
	M = glm::mat4(1); 

	lightPos = glm::vec4(10,10,10,1);

	boat = new Boat( 
		new Model("Models/boat.obj", P, V, M, "vshader.txt", "fshader.txt"),
		new Cloth("Models/sail.obj", P, V, M, "vshader.txt", "fshader.txt")
		);
}

void InitializeGame(int argc, char **argv){

	InitOpenGL(45.0f, 800, 600);
	InitGLUT(&argc, argv);
	InitGLEW();
	Initialize();

}

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

		printf("OpenGL 3.3 NOT supported\n");
		getchar();
		exit(0);

	}

}

#pragma endregion

#pragma region Update

void Update(){

	M = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, -1, 0)), angle, glm::vec3(0,1,0));
	//M = glm::mat4(1);

	boat->RotateSail(sailAngle);
	boat->Update(P, V, M, lightPos);
	
	angle += 0.5f;
	sailAngle += adder;
	if(sailAngle > 60.0f || sailAngle < -60.0f)
		adder *= -1;

}

void KeyboardEvent(unsigned char c, int x, int y){

	if(c == 32){
	
		wind.z *= -1;
		boat->SetWind(wind);

	}

	if(c == 'a') sailAngle -= 5;
	if(c == 'd') sailAngle += 5;


	printf("%d\n", c);

	glutPostRedisplay();

}

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

void DisplayFrame(){

	glClearColor(0.3921f, 0.5843f, 0.9294f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw();

	glutSwapBuffers();

}

void Draw(){

	boat -> Draw();
	frames++;

}

#pragma endregion


