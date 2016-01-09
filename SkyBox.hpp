/** \file SkyBox.hpp
 * \author Tomasz Jakubczyk
 * \brief Klasa wyświetlania nieba, może również zawierać, ziemię, słońce
 * i statyczną scenerię
 */

#pragma once

#include <string>
#include <fstream>
#include <windows.h>
#include <gl/gl.h>
#include <GL/glut.h>

class SkyBox
{
public:
	/** Default constructor */
	SkyBox(const string& Directory,
	const string& PosXFilename,
	const string& NegXFilename,
	const string& PosYFilename,
	const string& NegYFilename,
	const string& PosZFilename,
	const string& NegZFilename);
	~SkyBox();
	void operator()();
	bool Load();
	void Bind(GLenum TextureUnit);
protected:
private:
	string m_fileNames[6];
	GLuint m_textureObj;
};
