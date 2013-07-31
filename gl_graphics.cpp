#include "stdafx.h"
#include "gl_graphics.h"

GLvoid glCuboid3d(double x, double y, double z) {
	glBegin(GL_QUADS);									// Draw A Quad
		glVertex3d( x, y,-z);					// Top Right Of The Quad (Top)
		glVertex3d(-x, y,-z);					// Top Left Of The Quad (Top)
		glVertex3d(-x, y, z);					// Bottom Left Of The Quad (Top)
		glVertex3d( x, y, z);					// Bottom Right Of The Quad (Top)

		glVertex3d( x,-y, z);					// Top Right Of The Quad (Bottom)
		glVertex3d(-x,-y, z);					// Top Left Of The Quad (Bottom)
		glVertex3d(-x,-y,-z);					// Bottom Left Of The Quad (Bottom)
		glVertex3d( x,-y,-z);					// Bottom Right Of The Quad (Bottom)

		glVertex3d( x, y, z);					// Top Right Of The Quad (Front)
		glVertex3d(-x, y, z);					// Top Left Of The Quad (Front)
		glVertex3d(-x,-y, z);					// Bottom Left Of The Quad (Front)
		glVertex3d( x,-y, z);					// Bottom Right Of The Quad (Front)

		glVertex3d( x,-y,-z);					// Top Right Of The Quad (Back)
		glVertex3d(-x,-y,-z);					// Top Left Of The Quad (Back)
		glVertex3d(-x, y,-z);					// Bottom Left Of The Quad (Back)
		glVertex3d( x, y,-z);					// Bottom Right Of The Quad (Back)

		glVertex3d(-x, y, z);					// Top Right Of The Quad (Left)
		glVertex3d(-x, y,-z);					// Top Left Of The Quad (Left)
		glVertex3d(-x,-y,-z);					// Bottom Left Of The Quad (Left)
		glVertex3d(-x,-y, z);					// Bottom Right Of The Quad (Left)

		glVertex3d( x, y,-z);					// Top Right Of The Quad (Right)
		glVertex3d( x, y, z);					// Top Left Of The Quad (Right)
		glVertex3d( x,-y, z);					// Bottom Left Of The Quad (Right)
		glVertex3d( x,-y,-z);					// Bottom Right Of The Quad (Right)
	glEnd();
}
GLvoid glSquare3d(double x, double y, double z) {
	glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0); glVertex3d(-x, -y, z);  // Bottom Left Of The Texture and Quad
		glTexCoord2d(1.0, 0.0); glVertex3d( x, -y, z);  // Bottom Right Of The Texture and Quad
		glTexCoord2d(1.0, 1.0); glVertex3d( x,  y, z);  // Top Right Of The Texture and Quad
		glTexCoord2d(0.0, 1.0); glVertex3d(-x, y, z);  // Top Left Of The Texture and Quad
	glEnd();
}
GLvoid glPrint(const char *fmt, GLuint base, GLYPHMETRICSFLOAT* gmf) { // Custom GL "Print" Routine
	float		length=0;				// Used To Find The Length Of The Text
	char		text[256];				// Holds Our String
	va_list		ap;						// Pointer To List Of Arguments
	if (fmt == NULL)					// If There's No Text
		return;							// Do Nothing
	va_start(ap, fmt);					// Parses The String For Variables
	    vsprintf(text, fmt, ap);		// And Converts Symbols To Actual Numbers
	va_end(ap);							// Results Are Stored In Text

	for (unsigned int loop=0;loop<(strlen(text));loop++) {	// Loop To Find Text Length
		length+=gmf[text[loop]].gmfCellIncX;				// Increase Length By Each Characters Width
	}
	glTranslatef(-length/2,0.0f,0.0f);						// Center Our Text On The Screen
	glPushAttrib(GL_LIST_BIT);								// Pushes The Display List Bits
	glListBase(base);										// Sets The Base Character to 0
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);		// Draws The Display List Text
	glPopAttrib();											// Pops The Display List Bits
}
GLvoid glCircle (double r, double f) 
{
    float x, y, angle;
	glBegin(GL_LINES);
    for (int i = 0; i < ((int)f)%360; i++)
    {
		angle = (float)(i*M_PI/180);
		x = (float)(r * cos(angle));
		y = (float)(r * sin(angle));
		glVertex3f(x,y,0);
		angle = (float)((i+1)*M_PI/180);
		x = (float)(r * cos(angle));
		y = (float)(r * sin(angle));
		glVertex3f(x,y,0);
    }
    glEnd();
}

GLvoid glCircle_Arrow(double r)
{
	glPushMatrix();
		glRotatef(90,0,1,0);
		glCircle(r,270);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-0.0f,-0.0f,-(float)r);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f,0.0f,0.2f);
			glVertex3f(0.0f,-0.4f,0.0f);
			glVertex3f(0.0f,0.0f,-0.2f);
		glEnd();
	glPopMatrix();
}