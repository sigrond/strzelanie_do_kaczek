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
#include <string>

class SkyBox
{
public:
	/** Default constructor */
	SkyBox(const std::string& Directory,
	const std::string& PosXFilename,//prawo
	const std::string& NegXFilename,//lewo
	const std::string& PosYFilename,//góra
	const std::string& NegYFilename,//dół
	const std::string& PosZFilename,//tył?
	const std::string& NegZFilename);//przód?
	~SkyBox();
	void operator()();
	bool Load();
	void Bind(GLenum TextureUnit);
protected:
private:
	std::string m_fileNames[6];
	GLuint m_textureObj;
};
