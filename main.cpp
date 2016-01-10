/** \file main.cpp
 * \author Tomasz Jakubczyk
 * \brief Plik główny
 *
 */

#include <windows.h>
#include <gl/gl.h>
#include <GL/glut.h>
#include <gl/freeglut.h>
#include "SkyBox.hpp"
#include "Kaczka.hpp"
#include "Strzal.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

float oldMouseX=0;
float oldMouseY=0;
float mouseX=0;
float mouseY=0;
float lookX=0, lookY=0, lookZ=0;
float angle=0;
float zoomX=2,zoomY=2,zoomZ=2;
bool rmbd=false;
float moveX=0,moveZ=0;

SkyBox skyBox("mudriver\\",
				"valley_right.bmp",
				"valley_left.bmp",
				"valley_top.bmp",
				"valley_bottom.bmp",
				"valley_back.bmp",
				"valley_front.bmp");

Kaczka* kaczka;

float x = 0;
float y = 0;
float z = 0;
float width  = 1024;
float height = 1024;
float length = 1024;

float cameraX = lookX + mouseX / 2.0f;
float cameraY = lookY + mouseY / 2.0f;
float cameraZ = lookZ;

float theta = 0.0f;

WNDCLASSEX wcex;
HWND hwnd;
HDC hDC;
HGLRC hRC;
MSG msg;
BOOL bQuit = FALSE;


void display()
{
	//cout<<"display"<<endl;

	//glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = {.5, 1., 1., 0.};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/*gluLookAt(
		0,0.1,0.5,
		//camera->x(),camera->y(),camera->z(),
		0,0,0.6,
		0.1,0,0);*/
	//glScalef(1, 1, 1.001);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);  // switch to projection matrix
	glLoadIdentity();  // reset projection
	gluPerspective(45.0,4.0/3.0,0.01,10000.0);   // 90deg FOV, 4:3 aspect ratio, 0.01 near clip plane, 10.0 far clip plane
	glMatrixMode(GL_MODELVIEW);  // back to model matrix
	float cameraY = lookY + mouseY / 2.0f;
	glTranslatef(moveX,0,moveZ);
	glRotatef(mouseY/8-30, 1.0f, 0.0f, 0.0f);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	if(rmbd)
	{
		glScalef(zoomX/2, zoomY/2, zoomZ/2);
	}
	else
	{
		glScalef(zoomX, zoomY, zoomZ);
	}
	//glLoadIdentity();
	// Enable/Disable features
	skyBox.Render(1.0f);
	kaczka->renderKaczka(0.1f);
	//glTranslatef(0,0,-1.0f);
	//glColor4f(1,0,0,0.8);
	//glutSolidSphere(0.1f,20,20);
	glPopMatrix();
	theta += 0.1f;
	glFlush();
}


void ReSizeGLScene(int Width, int Height)
{
	if (Height==0)                                // Prevent A Divide By Zero If The Window Is Too Small
		Height=1;
	glViewport(0, 0, Width, Height);                // Reset The Current Viewport And Perspective Transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	cout<<"resized"<<endl;
}

void keyPressed(unsigned char key, int x, int y)
{
	cout<<"keyPressed"<<endl;
	switch (key)
	{
		case 'q':
			cout<<"Q"<<endl;
			angle-=3;
			break;
		case 'e':
			cout<<"E"<<endl;
			angle+=3;
			break;
		case 'a':
			cout<<"A"<<endl;
			if(moveX<1)
			moveX+=0.01;
			break;
		case 'd':
			cout<<"D"<<endl;
			if(moveX>-1)
			moveX-=0.01;
			break;
		case 'w':
			cout<<"W"<<endl;
			if(moveZ<1)
			moveZ+=0.01;
			break;
		case 's':
			cout<<"S"<<endl;
			if(moveZ>-1)
			moveZ-=0.01;
			break;
	}

    /* If 'x' is pressed, kill everything. */
    if (key == 'x')
    {
      /* shut down our window */
      //glutDestroyWindow(window);

      /* exit the program...normal termination. */
      exit(0);
    }
}

void mButtonPressed(int button, int state, int x, int y)
{
	cout<<"button: "<<button<<endl;
	cout<<"state: "<<state<<endl;
	if(button==2 && state==0)
	{
		rmbd=true;
	}
	else
	{
		rmbd=false;
	}
	if(button=2 && state==0)
	{
		if(Strzal(moveX,-1,moveZ,mouseY/8-30,angle,kaczka->getCartPos()))
		kaczka->trafiona();
	}
}

void mouseMove(int x, int y)
{
	// save old mouse coordinates
	oldMouseX = mouseX;
	oldMouseY = mouseY;

	// get mouse coordinates from Windows
	mouseX = x;
	mouseY = y;

	// these lines limit the camera's range
	if (mouseY < 60)
		mouseY = 60;
	if (mouseY > 450)
		mouseY = 450;

	if ((mouseX - oldMouseX) > 0)		// mouse moved to the right
		angle += 3.0f;
	else if ((mouseX - oldMouseX) < 0)	// mouse moved to the left
		angle -= 3.0f;
}

void init()
{
    glClearColor(0, 0, 0, 0); // moved this line to be in the init function
    //glDisable(GL_DEPTH_TEST);
    //glEnable(GL_DEPTH_TEST);

    glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // next four lines are new
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width-1, height-1, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(width, height);
    glutCreateWindow( "Strzelanie do kaczek" );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	skyBox.Load();
	kaczka=new Kaczka("duck\\",
					"duck_up.bmp",
					"duck_mid.bmp",
					"duck_down.bmp");
	kaczka->load();
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;
	glTranslatef(0,-1,0);
	glRotatef(15, 1.0f, 0.0f, 0.0f);
	glScalef(zoomX, zoomY, zoomZ);
	glutReshapeFunc(ReSizeGLScene);
	glutFullScreen();
	glutKeyboardFunc(keyPressed);
	glutMouseFunc(mButtonPressed);
	glutPassiveMotionFunc(mouseMove);
	glutIdleFunc(display);
	init();
	glutDisplayFunc( display );
	glutMainLoop();

	gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 0.0, 1.0, 0.0);

    /* shutdown OpenGL */
    //DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    //DestroyWindow(hwnd);

    return 0;
}



