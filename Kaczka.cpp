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
		speed(0.1f), state(StanKaczki::UP), licznik(0),
		skrzydla(0), obrotZ(10)
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
		obrotZ=-10;
		if(y>=1.0f)
		{
			state=StanKaczki::DOWN;
		}
	}
	else if(state==StanKaczki::DOWN)
	{
		//theta-=0.0001f;
		y-=0.001f;
		obrotZ=10;
		if(y<=-1.0f)
		{
			state=StanKaczki::UP;
		}
	}
	else if(state==StanKaczki::DEAD)
	{
		y-=0.002f;
		skrzydla=1;
		obrotZ+=0.1f;
		if(obrotZ>90)
			obrotZ=90;
		if(y<=-2.0f)
		{
			state=StanKaczki::UP;
			phi-=3.1415f;
		}
			//return;
	}
	//x=rho*cos(phi)*cos(theta);
	//y=rho*cos(phi)*sin(theta);
	//z=rho*sin(phi);
	x=rho*sin(phi);
	z=rho*cos(phi);
	glTranslatef(x,y,z);
	glRotatef(phi*180/3.1415f,0,1,0);
	glRotatef(-obrotZ,0,0,1);

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glColor4f(1,0,0,0.8);
	glBindTexture(GL_TEXTURE_2D, textureID[skrzydla]);
	//glutSolidSphere(0.1f,20,20);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( -size, -size/2, -size );
		glTexCoord2f(1, 0); glVertex3f(  size, -size/2, -size );
		glTexCoord2f(1, 1); glVertex3f(  size,  size/2, -size );
		glTexCoord2f(0, 1); glVertex3f( -size,  size/2, -size );
	glEnd();
}

void Kaczka::load()
{
	Image* image=NULL;
	char* data=NULL;
	for(int i=0;i<3;i++)
	{
		image=loadBMP_custom(fileNames[i].c_str());
		//własna alpha
		data=new char[image->width*image->height*4];
		for(int j=0;j<image->width*image->height;j++)
		{
			//cout<<image->data[3*j]<<" "<<image->data[3*j+1]<<" "<<image->data[3*j+2]<<endl;
			if(image->data[3*j]==0xFF && image->data[3*j+1]==0xFF && image->data[3*j+2]==0xFF)
			{
				data[4*j]=0xFF;
				data[4*j+1]=0xFF;
				data[4*j+2]=0xFF;
				data[4*j+3]=0x00;
			}
			else if(image->data[3*j]>0xEE && image->data[3*j+1]>0xEE && image->data[3*j+2]>0xEE)
			{
				data[4*j]=image->data[3*j];
				data[4*j+1]=image->data[3*j+1];
				data[4*j+2]=image->data[3*j+2];
				data[4*j+3]=0x88;
			}
			else
			{
				data[4*j]=image->data[3*j];
				data[4*j+1]=image->data[3*j+1];
				data[4*j+2]=image->data[3*j+2];
				data[4*j+3]=0xFF;
			}
		}
		glGenTextures(1, &textureID[i]);
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID[i]);
		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, image->width, image->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
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
		if(data!=NULL)
			delete[] data;
		data=NULL;
	}
}

bool Kaczka::trafiona()
{
	state=StanKaczki::DEAD;
}

float* Kaczka::getCartPos()
{
	float* r=new float[3];
	r[0]=x;
	r[1]=y;
	r[2]=z;
	return r;
}



















