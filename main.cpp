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
#include <cstdlib>

using namespace std;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


int main(int argc, char* argv[])
{
	HINSTANCE hInstance=GetModuleHandle(NULL);

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1024,
                          1024,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, SW_SHOW);

    //char *argv[] = {"foo", "bar"};
	//int argc = 2;
    //glutInit(&__argc,__argv);
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );



    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

	SkyBox skyBox("mudriver\\",
				"valley_right.bmp",
				"valley_left.bmp",
				"valley_top.bmp",
				"valley_bottom.bmp",
				"valley_back.bmp",
				"valley_front.bmp");

	skyBox.Load();

	float x = 0;
	float y = 0;
	float z = 0;
	float width  = 1024;
	float height = 1024;
	float length = 1024;

	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	glScalef(2, 2, 2);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

			/*gluLookAt(
				0,0,0.5,
				//camera->x(),camera->y(),camera->z(),
				0,0,0.6,
				0,0,0);*/
            //glScalef(1, 1, 1.001);
            glPushMatrix();

            glMatrixMode(GL_PROJECTION);  // switch to projection matrix
			glLoadIdentity();  // reset projection
			gluPerspective(110.0,4.0/3.0,0.01,10.0);   // 90deg FOV, 4:3 aspect ratio, 0.01 near clip plane, 10.0 far clip plane
			glMatrixMode(GL_MODELVIEW);  // back to model matrix

            glRotatef(theta, 0.0f, 1.0f, 0.0f);
            //gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
            //glLoadIdentity();

			//glColor4f(1,0,0,0.8);
            //glutSolidSphere(0.1,20,20);

			// Enable/Disable features
			glPushAttrib(GL_ENABLE_BIT);
			glEnable(GL_TEXTURE_2D);
			//glDisable(GL_DEPTH_TEST);
			//glDisable(GL_LIGHTING);
			//glDisable(GL_BLEND);
			//glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
			//glEnable(GL_TEXTURE_GEN_T);

			// Just in case we set all vertices to white.
			glColor4f(1,1,1,1);

			// Render the front quad
			glBindTexture(GL_TEXTURE_2D, skyBox.textureID[5]);
			//glColor3f(1,0,0);
			glBegin(GL_QUADS);
				/*glTexCoord2f(0, 0); glVertex3f(  1.0f, -1.0f, -1.0f );
				glTexCoord2f(1, 0); glVertex3f( -1.0f, -1.0f, -1.0f );
				glTexCoord2f(1, 1); glVertex3f( -1.0f,  1.0f, -1.0f );
				glTexCoord2f(0, 1); glVertex3f(  1.0f,  1.0f, -1.0f );*/
				glTexCoord2f(0, 0); glVertex3f(  -1.0f, -1.0f, -1.0f );
				glTexCoord2f(1, 0); glVertex3f( 1.0f, -1.0f, -1.0f );
				glTexCoord2f(1, 1); glVertex3f( 1.0f,  1.0f, -1.0f );
				glTexCoord2f(0, 1); glVertex3f(  -1.0f,  1.0f, -1.0f );


			glEnd();

			// Render the right quad
			glBindTexture(GL_TEXTURE_2D, skyBox.textureID[0]);
			glBegin(GL_QUADS);
				/*glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
				glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
				glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
				glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f,  0.5f );*/
				glTexCoord2f(0, 0); glVertex3f(  1.0f, -1.0f,  -1.0f );
				glTexCoord2f(1, 0); glVertex3f(  1.0f, -1.0f, 1.0f );
				glTexCoord2f(1, 1); glVertex3f(  1.0f,  1.0f, 1.0f );
				glTexCoord2f(0, 1); glVertex3f(  1.0f,  1.0f,  -1.0f );
			glEnd();

			// Render the back quad
			glBindTexture(GL_TEXTURE_2D, skyBox.textureID[4]);
			//glColor3f(0,0,0);
			glBegin(GL_QUADS);
				/*glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
				glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
				glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
				glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f,  0.5f );*/
				glTexCoord2f(0, 0); glVertex3f( 1.0f, -1.0f,  1.0f );
				glTexCoord2f(1, 0); glVertex3f(  -1.0f, -1.0f,  1.0f );
				glTexCoord2f(1, 1); glVertex3f(  -1.0f,  1.0f,  1.0f );
				glTexCoord2f(0, 1); glVertex3f( 1.0f,  1.0f,  1.0f );

			glEnd();

			// Render the left quad
			//glColor3f(0,1,0);
			glBindTexture(GL_TEXTURE_2D, skyBox.textureID[1]);
			glBegin(GL_QUADS);
				/*glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
				glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
				glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
				glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );*/
				glTexCoord2f(0, 0); glVertex3f( -1.0f, -1.0f, 1.0f );
				glTexCoord2f(1, 0); glVertex3f( -1.0f, -1.0f,  -1.0f );
				glTexCoord2f(1, 1); glVertex3f( -1.0f,  1.0f,  -1.0f );
				glTexCoord2f(0, 1); glVertex3f( -1.0f,  1.0f, 1.0f );
			glEnd();

			// Render the top quad
			//glColor3f(0,0,1);
			glBindTexture(GL_TEXTURE_2D, skyBox.textureID[2]);
			glBegin(GL_QUADS);
				/*glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
				glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
				glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
				glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );*/
				glTexCoord2f(0, 1); glVertex3f(  -1.0f,  1.0f, 1.0f );
				glTexCoord2f(0, 0); glVertex3f(  -1.0f,  1.0f,  -1.0f );
				glTexCoord2f(1, 0); glVertex3f( 1.0f,  1.0f,  -1.0f );
				glTexCoord2f(1, 1); glVertex3f( 1.0f,  1.0f, 1.0f );
			glEnd();

			// Render the bottom quad
			//glColor3f(1,0,0);
			glBindTexture(GL_TEXTURE_2D, skyBox.textureID[3]);
			glBegin(GL_QUADS);
				/*glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
				glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
				glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
				glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );*/
				glTexCoord2f(0, 0); glVertex3f( -1.0f, -1.0f, 1.0f );
				glTexCoord2f(0, 1); glVertex3f( -1.0f, -1.0f,  -1.0f );
				glTexCoord2f(1, 1); glVertex3f(  1.0f, -1.0f,  -1.0f );
				glTexCoord2f(1, 0); glVertex3f(  1.0f, -1.0f, 1.0f );

				//glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
				//glTexCoord2f(0, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
				//glTexCoord2f(1, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
				//glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
			glEnd();

			// Restore enable bits and matrix
			glPopAttrib();

            glPopMatrix();

            SwapBuffers(hDC);

            theta += 0.1f;
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);


}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

