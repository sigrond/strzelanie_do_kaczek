/** \file Kaczka.cpp
 * \author Tomasz Jakubczyk
 * \brief Plik nagłówkowy klasy Kaczka
 *
 */

#include "Kaczka.hpp"
#include <gl/freeglut.h>
#include <cmath>
#include "openBMP.hpp"
#include <iostream>

using namespace std;

Kaczka::Kaczka(const std::string& Directory,
	const std::string& DuckUpFilename,
	const std::string& DuckMidFilename,
	const std::string& DuckDownFilename) :
		x(0), y(0), z(-1.0f), phi(0), theta(0), rho(2.5f),
		speed(0.1f), state(StanKaczki::UP), licznik(0), skrzydla(0)
{
	fileNames[0]=Directory+DuckUpFilename;
	fileNames[1]=Directory+DuckMidFilename;
	fileNames[2]=Directory+DuckDownFilename;
}

Kaczka::~Kaczka()
{
	//dtor
}

void Kaczka::renderKaczka(float size)
{
	phi+=0.001f;
	//cout<<licznik<<endl;
	if(++licznik%100==1)
	{
		skrzydla++;
	}
	if(skrzydla>=3)
	{
		skrzydla=0;
	}
	if(state==StanKaczki::UP)
	{
		//theta+=0.0001f;
		y+=0.001f;
		if(y>=1.0f)
		{
			state=StanKaczki::DOWN;
		}
	}
	else if(state==StanKaczki::DOWN)
	{
		//theta-=0.0001f;
		y-=0.001f;
		if(y<=-1.0f)
		{
			state=StanKaczki::UP;
		}
	}
	//x=rho*cos(phi)*cos(theta);
	//y=rho*cos(phi)*sin(theta);
	//z=rho*sin(phi);
	x=rho*sin(phi);
	z=rho*cos(phi);
	glTranslatef(x,y,z);
	glRotatef(phi*180/3.1415f,0,1,0);

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);

	//glColor4f(1,0,0,0.8);
	glBindTexture(GL_TEXTURE_2D, textureID[skrzydla]);
	//glutSolidSphere(0.1f,20,20);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( -size, -size, -size );
		glTexCoord2f(1, 0); glVertex3f(  size, -size, -size );
		glTexCoord2f(1, 1); glVertex3f(  size,  size, -size );
		glTexCoord2f(0, 1); glVertex3f( -size,  size, -size );
	glEnd();
}

void Kaczka::load()
{
	Image* image=NULL;
	for(int i=0;i<3;i++)
	{
		image=loadBMP_custom(fileNames[i].c_str());
		glGenTextures(1, &textureID[i]);
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID[i]);
		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, image->width, image->height, 0, GL_BGR, GL_UNSIGNED_BYTE, image->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//te dwa były potrzebne, żeby tekstury się wyświetlały
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(image->data!=NULL)
			delete[] image->data;
		image->data=NULL;
		if(image!=NULL)
			delete image;
		image=NULL;
	}
}





















