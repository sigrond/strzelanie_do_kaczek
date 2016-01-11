/** \file Shotgun.cpp
 * \author Tomasz Jakubczyk
 * \brief Plik z implementacjami metod klasy Shotgun
 *
 */

#include "Shotgun.hpp"
#include "openBMP.hpp"

Shotgun::Shotgun(const std::string& Directory,
				const std::string& ShotgunFilename)
{
	fileName=Directory+ShotgunFilename;
}

void Shotgun::load()
{
	Image* image=NULL;
	char* data=NULL;
	image=loadBMP_custom(fileName.c_str());
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
	glGenTextures(1, &textureID);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, image->width, image->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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

void Shotgun::renderShotgun(float phi,float theta,float size)
{
	glPushMatrix();

	glMatrixMode (GL_PROJECTION); // Tell opengl that we are doing project matrix work
	glLoadIdentity(); // Clear the matrix
	gluPerspective(60.0,4.0/3.0,0.01,100.0);
	//glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0); // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW); // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix

	//glDisable(GL_COLOR_MATERIAL);
	//glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);

	//gluPerspective(60.0,4.0/3.0,0.01,100.0);   // 90deg FOV, 4:3 aspect ratio, 0.01 near clip plane, 10.0 far clip plane

	//glRotatef(-theta, 1.0f, 0.0f, 0.0f);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	//glRotatef(10, 0.0f, 0.0f, 1.0f);
	//glScalef(2,2,2);
	glTranslatef(-size*1.25f,-size,-size);


	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glColor4f(1,0,0,0.8);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//glutSolidSphere(0.1f,20,20);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( -size, -size/2, -size );
		glTexCoord2f(1, 0); glVertex3f(  size, -size/2, -size );
		glTexCoord2f(1, 1); glVertex3f(  size,  size/2, -size );
		glTexCoord2f(0, 1); glVertex3f( -size,  size/2, -size );
	glEnd();
	glPopMatrix();
}

























