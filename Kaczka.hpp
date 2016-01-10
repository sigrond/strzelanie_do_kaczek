/** \file Kaczka.hpp
 * \author Tomasz Jakubczyk
 * \brief Klasa wyświetlania kaczki
 *
 */

#pragma once

#include <string>
#include <gl/gl.h>
#include <GL/glext.h>
#include <gl/freeglut.h>

namespace StanKaczki
{
	enum StanKaczki
	{
		UP,
		DOWN,
		DEAD,
	};
}

class Kaczka
{
public:
	/** Default constructor */
	Kaczka(const std::string& Directory,
	const std::string& DuckUpFilename,
	const std::string& DuckMidFilename,
	const std::string& DuckDownFilename);
	/** Default destructor */
	~Kaczka();
	void renderKaczka(float size=0.1f);
	void load();
	bool trafiona();
	float* getCartPos();
protected:
private:
	float x,y,z;/**< wsp kartezjańskie */
	//wsp. sferyczne :D
	float phi;//kierunek na boki
	float theta;//kierunek w pionie
	float rho;//promień
	float speed;
	StanKaczki::StanKaczki state;//stan kaczki
	std::string fileNames[3];
	GLuint textureID[3];
	unsigned long licznik;
	int skrzydla;
	float obrotZ;
};
