#include "StdAfx.h"
#include "GLWindow.h"
#include <iostream>

#pragma region Static fields

bool GLWindow::fullscreen = false;
bool GLWindow::active = false;
bool GLWindow::done = false;
bool *GLWindow::keys = new bool[256];
bool *GLWindow::previousKeys = new bool[256];

#pragma endregion

#pragma region Constructor, destructor, CreateGLWindow, WndProc 

GLWindow::GLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreen)
	: title(title), width(width), height(height), bits(bits){

	hRc = NULL;
	hDc = NULL;
	hWnd = NULL;
	GLWindow::fullscreen = fullscreen;
	GLWindow::active = active;

	textureCount = 2;
	textureImage = new Texture[textureCount];

	rotx = 0;
	roty = 0;
	rotz = 0;

	for(int i = 0; i < 256; i++){
		GLWindow::keys[i] = false;
		GLWindow::previousKeys[i] = false;
	}

	CreateGLWindow(title, width, height, bits, fullscreen);

	Initialize();

}

GLWindow::~GLWindow(){

	KillGLWindow();

}

BOOL GLWindow::CreateGLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreen){

	GLuint pixelFormat;
	WNDCLASS wc;

	DWORD dwExStyle;
	DWORD dwStyle;

	RECT windowRect;
	windowRect.left = 0;
	windowRect.right = (long)width;
	windowRect.top = 0;
	windowRect.bottom = (long)height;

	this->fullscreen = fullscreen;

	hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) GLWindow::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wc.hCursor = LoadCursor(hInstance, IDC_UPARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"OpenGL";

	if(!RegisterClass(&wc)){

		MessageBox(NULL, L"Could not register class", L"Error", MB_OK | MB_ICONINFORMATION);
		return FALSE;

	}

	if(fullscreen){

		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PAPERWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dmScreenSettings, 4) != DISP_CHANGE_SUCCESSFUL)
			fullscreen = false;

	}

	if(fullscreen){

		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);

	} else {
	
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;

	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	if(!(hWnd = CreateWindowEx(dwExStyle, wc.lpszClassName, title, dwStyle, 0, 0, width, height, NULL, NULL, hInstance, NULL))){

		MessageBox(NULL, L"Window Creation Error.", L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;

	}

	static	PIXELFORMATDESCRIPTOR pfd =				
	{
		sizeof(PIXELFORMATDESCRIPTOR),					
		1,								
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,						
		PFD_TYPE_RGBA,							
		bits,								
		0, 0, 0, 0, 0, 0,						
		0,								
		0,								
		0,								
		0, 0, 0, 0,							
		16,								
		0,								
		0,								
		PFD_MAIN_PLANE,							
		0,								
		0, 0, 0								
	};

	if(!(hDc = GetDC(hWnd))){
	
		MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;

	}

	if(!(pixelFormat = ChoosePixelFormat(hDc, &pfd))){
	
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;

	}

	if(!SetPixelFormat(hDc, pixelFormat, &pfd)){
	
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;

	}

	if(!(hRc = wglCreateContext(hDc))){
	
		MessageBox(NULL, L"Can't create RC.", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;

	}

	if(!wglMakeCurrent(hDc, hRc)){
	
		MessageBox(NULL, L"Can't activate RC.", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;

	}

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	ResizeGLScene(width, height);

	return TRUE;	

}

LRESULT GLWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	switch(uMsg){
	
	case WM_ACTIVATE:

		if(!HIWORD(wParam)) active = true;
		else active = false;
		return 0;

	case WM_SYSCOMMAND:

		switch(wParam){
		
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;

		}
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		GLWindow::keys[wParam] = true;
		return 0;

	case WM_KEYUP:
		GLWindow::keys[wParam] = false;
		return 0;

	case WM_SIZE:
		GLWindow::ResizeGLScene(LOWORD(lParam), HIWORD(wParam));
		return 0;

	case WM_QUIT:
		GLWindow::done = true;
		return 0;
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#pragma endregion

#pragma region Initialization

void GLWindow::InitGL(){

	
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f); //cornflower blue
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat ambientLight[] =  { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseLight[] =  { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat lightPosition[] = { 10.0f, 0.0f, 10.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
		
}

void GLWindow::Initialize(){

	InitGL(); 
	LoadContent();
	
}

#pragma endregion

#pragma region LoadContent

void GLWindow::LoadContent(){
	
	LoadBitMap(L"wood.bmp", &(textureImage[0]), "wood.png");
	LoadBitMap(L"woodplanks.bmp", &(textureImage[1]), "woodplanks.jpg");
	LoadFont(L"Courier New");
	boat.Load("boat.obj", textureImage, textureCount);
	//mikeBoat.Load("mikeBoat.obj", textureImage, textureCount);
	cloth.Load("sail.obj", textureImage, textureCount);
	cloth.BuildCloth();

}

void GLWindow::LoadBitMap(LPTSTR fileName, Texture *texture, string name){

	HBITMAP hBMP;
	BITMAP	BMP;

	GLuint texid;
	glGenTextures(1, &texid);
	hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL), fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );

	texture -> tex = texid;
	texture -> name = name;

	if (!hBMP) return;	

	GetObject(hBMP, sizeof(BMP), &BMP);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);

	
}

void GLWindow::LoadFont(LPCWSTR fontName){
	
	HFONT font;
	HFONT oldfont;

	base = glGenLists(96);
	font = CreateFont(
		-24,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		fontName
		);
	oldfont = (HFONT)SelectObject(hDc, font);
	wglUseFontBitmaps(hDc, 32, 96, base);
	SelectObject(hDc, oldfont);
	DeleteObject(font);

}

#pragma endregion

#pragma region Update logic

void GLWindow::StartLoop(){

	done = false;

	int frameCounter = 0;

	clock_t start = clock();
	clock_t fpsShow = clock() + CLOCKS_PER_SEC;
	clock_t now = clock(), before = clock();
	MSG	msg;
	while(!GLWindow::done){

		now = clock();
		
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
		
			TranslateMessage(&msg);				
			DispatchMessage(&msg);

		}

		Update();
		DrawGLScene();
		SwapBuffers(hDc);
		frameCounter++;

		if(now > fpsShow){

			fpsShow = now + CLOCKS_PER_SEC;
			fps = (GLfloat) frameCounter/(now-before)*CLOCKS_PER_SEC;
			std::cout << (double) fps  << std::endl;
			frameCounter = 0;
			before = now;

		}

	}

}

void GLWindow::Update(){

	KeyboardHandle();
	rotx += 0.3f;
	roty += 0.3f;
	rotz += 0.3f;

	cloth.TimeStep();
	 
}

void GLWindow::KeyboardHandle(){

	bool *keyboard = GLWindow::keys;
	bool *previousKeyboard = GLWindow::previousKeys;

	if(keyboard[VK_F11]){

		ToggleFullscreen();
		keyboard[VK_F11] = false;
	}

	if(keyboard[VK_ESCAPE])
		done = true;

	if(keyboard[VK_SPACE] && !previousKeyboard[VK_SPACE]) 
		cloth.FlipWind();

	if(keyboard[78] && !previousKeyboard[78]) 
		cloth.ToogleNormals();

	for(int i = 0; i < 256; i++)
		GLWindow::previousKeys[i] = GLWindow::keys[i];

}

#pragma endregion

#pragma region Drawing

int GLWindow::DrawGLScene(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(0.0f, -1.5f, -7.0f);

	//glRotatef(rotx, 1.0f, 0.0f, 0.0f);
	glRotatef(roty, 0.0f, 1.0f, 0.0f);
	//glRotatef(rotz, 0.0f, 0.0f, 1.0f);
	cloth.Draw();
	
	for(unsigned i = 0; i < boat.meshes.size(); i++)
		boat.meshes[i].Draw();
	

	//Tu mike Twoja lodka
	
	/*
	
	glLoadIdentity();
	glTranslatef(0.0f, -1.0f, -15.0f);

	glRotatef(rotx, 1.0f, 0.0f, 0.0f);
	glRotatef(roty, 0.0f, 1.0f, 0.0f);
	glRotatef(rotz, 0.0f, 0.0f, 1.0f);

	for(unsigned i = 0; i < mikeBoat.meshes.size(); i++)
		mikeBoat.meshes[i].Draw();
	*/


	return 0;

}

void GLWindow::printMessage(const char *msg, ...){

	char text[256];
	va_list arguments;

	if(msg == NULL) return;

	va_start(arguments, msg);
	vsprintf_s(text, msg, arguments);
	va_end(arguments);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();

}

#pragma endregion

#pragma region Additional methods
void GLWindow::ToggleFullscreen(){

	GLWindow::fullscreen = !GLWindow::fullscreen;

		DWORD dwExStyle, dwStyle;
		
		int w, h;
		if(GLWindow::fullscreen){

			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
			ShowCursor(FALSE);
			w = GetSystemMetrics(SM_CXSCREEN);
			h = GetSystemMetrics(SM_CYSCREEN);

		} else {
	
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
			ShowCursor(TRUE);
			w = width;
			h = height;
			
		}

		SetWindowLong(hWnd, GWL_STYLE, dwStyle );
		SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
		
		SetWindowPos(hWnd, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

}


GLvoid GLWindow::ResizeGLScene(GLsizei width, GLsizei height){

	if(height == 0) height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void GLWindow::KillGLWindow(){

	if(fullscreen){
	
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);

	}

	if(hRc){
	
		if(!wglMakeCurrent(NULL, NULL))
			MessageBox(NULL, L"Release of DC and RC failed.", L"Shutdown error", MB_OK | MB_ICONINFORMATION);


		if(!wglDeleteContext(hRc))
			MessageBox(NULL, L"Release of RC failed.", L"Shutdown error", MB_OK | MB_ICONINFORMATION);
		hRc = NULL;

	}

	if(hDc && !ReleaseDC(hWnd, hDc))
		MessageBox(NULL, L"Release of DC failed.", L"Shutdown error", MB_OK | MB_ICONINFORMATION);
	hDc = NULL;

	if(hWnd && !DestroyWindow(hWnd))
		MessageBox(NULL, L"Release of HWND failed.", L"Shutdown error", MB_OK | MB_ICONINFORMATION);
	hWnd = NULL;

	if(!UnregisterClass(L"OpenGL", hInstance))
		MessageBox(NULL, L"Could not unregister class.", L"Shutdown error", MB_OK | MB_ICONINFORMATION);
	hInstance = NULL;

}

#pragma endregion