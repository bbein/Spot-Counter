#pragma once

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <stdio.h>			// Header File For Standard Input/Output	( ADD )
#define _USE_MATH_DEFINES
#include <math.h>

GLvoid glCuboid3d(double x, double y, double z);
GLvoid glSquare3d(double x, double y, double z);
GLvoid glPrint(const char *fmt, GLuint base, GLYPHMETRICSFLOAT* gmf);
GLvoid glCircle_Arrow(double r);
GLvoid glCircle (double r, double f);