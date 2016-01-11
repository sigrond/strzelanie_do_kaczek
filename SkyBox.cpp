/** \file SkyBox.cpp
 * \author Tomasz Jakubczyk
 * \brief Klasa wyświetlania nieba, może również zawierać, ziemię, słońce
 * i statyczną scenerię
 */

#include "SkyBox.hpp"
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <gl/gl.h>
#include <GL/glut.h>
#include <gl/glu.h>
#include <GL/glext.h>
#include <gl/freeglut.h>
#include <iostream>
#include "openBMP.hpp"

using namespace std;

SkyBox::SkyBox(const string& Directory,
	const string& PosXFilename,
	const string& NegXFilename,
	const string& PosYFilename,
	const string& NegYFilename,
	const string& PosZFilename,
	const string& NegZFilename)
{
	m_fileNames[0]=Directory+PosXFilename;
	m_fileNames[1]=Directory+NegXFilename;
	m_fileNames[2]=Directory+PosYFilename;
	m_fileNames[3]=Directory+NegYFilename;
	m_fileNames[4]=Directory+PosZFilename;
	m_fileNames[5]=Directory+NegZFilename;
}

SkyBox::~SkyBox()
{

}



int types[]={GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

bool SkyBox::Load()
{
	Image* image=NULL;

	//glGenTextures(1, &m_textureObj);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

	for(int i=0;i<6;i++)
	{
		image=loadBMP_custom(m_fileNames[i].c_str());

		// Create one OpenGL texture
		//GLuint textureID;
		glGenTextures(1, &textureID[i]);
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID[i]);
		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, image->width, image->height, 0, GL_BGR, GL_UNSIGNED_BYTE, image->data);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//te dwa były potrzebne, żeby tekstury się wyświetlały
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glTexImage2D(types[i], 0, GL_RGB, image->width, image->height, 0, GL_BGR,
		//GL_UNSIGNED_BYTE, image->data);
		//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		if(image->data!=NULL)
			delete[] image->data;
		image->data=NULL;
		if(image!=NULL)
			delete image;
		image=NULL;
	}
	return true;
}

void SkyBox::Bind(int i)
{
	//glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID[i]);
}

void SkyBox::Render(float size)
{
	glPushMatrix();
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
	glBindTexture(GL_TEXTURE_2D, textureID[5]);
	//glColor3f(1,0,0);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( -size, -size, -size );
		glTexCoord2f(1, 0); glVertex3f(  size, -size, -size );
		glTexCoord2f(1, 1); glVertex3f(  size,  size, -size );
		glTexCoord2f(0, 1); glVertex3f( -size,  size, -size );


	glEnd();

	// Render the right quad
	glBindTexture(GL_TEXTURE_2D, textureID[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(  size, -size, -size );
		glTexCoord2f(1, 0); glVertex3f(  size, -size,  size );
		glTexCoord2f(1, 1); glVertex3f(  size,  size,  size );
		glTexCoord2f(0, 1); glVertex3f(  size,  size, -size );
	glEnd();

	// Render the back quad
	glBindTexture(GL_TEXTURE_2D, textureID[4]);
	//glColor3f(0,0,0);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(  size, -size,  size );
		glTexCoord2f(1, 0); glVertex3f( -size, -size,  size );
		glTexCoord2f(1, 1); glVertex3f( -size,  size,  size );
		glTexCoord2f(0, 1); glVertex3f(  size,  size,  size );

	glEnd();

	// Render the left quad
	//glColor3f(0,1,0);
	glBindTexture(GL_TEXTURE_2D, textureID[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( -size, -size,  size );
		glTexCoord2f(1, 0); glVertex3f( -size, -size, -size );
		glTexCoord2f(1, 1); glVertex3f( -size,  size, -size );
		glTexCoord2f(0, 1); glVertex3f( -size,  size,  size );
	glEnd();

	// Render the top quad
	//glColor3f(0,0,1);
	glBindTexture(GL_TEXTURE_2D, textureID[2]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f( -size,  size,  size );
		glTexCoord2f(0, 0); glVertex3f( -size,  size, -size );
		glTexCoord2f(1, 0); glVertex3f(  size,  size, -size );
		glTexCoord2f(1, 1); glVertex3f(  size,  size,  size );
	glEnd();

	// Render the bottom quad
	//glColor3f(1,0,0);
	glBindTexture(GL_TEXTURE_2D, textureID[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( -size, -size,  size );
		glTexCoord2f(0, 1); glVertex3f( -size, -size, -size );
		glTexCoord2f(1, 1); glVertex3f(  size, -size, -size );
		glTexCoord2f(1, 0); glVertex3f(  size, -size,  size );
	glEnd();

	// Restore enable bits and matrix
	glPopAttrib();
	glPopMatrix();
}
