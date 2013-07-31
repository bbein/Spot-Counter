#pragma once

#include <shobjidl.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <cstring>
#include <ctime>
#include <fstream>
#include <complex>
#include <sstream>

class histogram
{
public:
	histogram(void);
	~histogram(void);
	GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	BOOL CreateGLWindow(wchar_t* title, int width, int height, int bits, WNDPROC WndProc, HINSTANCE hInstance);
	void DrawGLScene();
private:
	HDC			hDC;		// Private GDI Device Context
	HGLRC		hRC;		// Permanent Rendering Context
	HWND		hWnd;		// Holds Our Window Handle

	void Init(void);
	int InitGL(GLvoid);
	GLvoid KillGLWindow(GLvoid);

};

GLvoid glsquare3f(GLfloat x, GLfloat y, GLfloat z);
