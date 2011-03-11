#pragma once

class GLWindow
{


private:

#pragma region Fields

	HGLRC hRc;
	HDC hDc;
	HWND hWnd;
	HINSTANCE hInstance;

	GLuint base;

	LPCWSTR title;
	int width;
	int height;
	int bits;

	GLuint textureImage[1];

	static bool *keys;
	static bool *previousKeys;
	static bool active;
	static bool fullscreen;

	static bool done;

	bool fullscreenChanged;

	GLfloat fps;

	int part1;
	int part2;
	int p1;
	int p2;

	GLUquadricObj *quadratic;

	Model boat;

	GLfloat rotx, roty, rotz;

#pragma endregion

#pragma region Methods

	void InitGL();

	void Initialize();

	void KillGLWindow();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	static GLvoid ResizeGLScene(GLsizei width, GLsizei height);

	BOOL CreateGLWindow(LPCWSTR title, int width, int height, int bits, bool fullScreenFlag);

	int DrawGLScene();

	void Update();

	void KeyboardHandle();

	void ToggleFullscreen();

	void LoadContent();

	void LoadBitMap(LPTSTR fileName, GLuint &textid);

	void LoadFont(LPCWSTR fontName);

	void printMessage(const char *msg, ...);

#pragma endregion

public:

#pragma region Fields



#pragma endregion

#pragma region Methods

	GLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreen);

	~GLWindow();

	void StartLoop();

#pragma endregion

};

