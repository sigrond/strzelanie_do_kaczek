/** \file Shotgun.hpp
 * \author Tomasz Jakubczyk
 * \brief Plik nagłówkowy klasy Shotgun
 *
 */

#pragma once

#include <string>
#include <gl/gl.h>
#include <GL/glext.h>
#include <gl/freeglut.h>

class Shotgun
{
public:
	/** Default constructor */
	Shotgun(const std::string& Directory,
			const std::string& ShotgunFilename);
	void load();
	void renderShotgun(float phi,float theta,float size=0.25f);
protected:
private:
	std::string fileName;
	GLuint textureID;
};
