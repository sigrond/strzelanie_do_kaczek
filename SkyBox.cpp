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

struct Image
{
	unsigned int width, height;
	unsigned char * data;
};

Image* loadBMP_custom(const char * imagepath)
{
	Image* image=new Image();
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	FILE * file = fopen(imagepath,"rb");
	if (!file)
	{
		printf("Image could not be opened\n");
		return 0;
	}

	if ( fread(header, 1, 54, file)!=54 )
	{ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return 0;
	}

	if ( header[0]!='B' || header[1]!='M' )
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)
		imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)
		dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];
	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);
	//Everything is in memory now, the file can be closed
	fclose(file);

	image->data=data;
	image->height=height;
	image->width=width;

	cout<<"Wczytano: "<<imagepath<<endl;

	return image;

	// Create one OpenGL texture
	//GLuint textureID;
	//glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	//glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL
	/*glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
*/
    //delete data;
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

	glGenTextures(1, &m_textureObj);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

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

void SkyBox::Render()
{

}
