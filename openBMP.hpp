/** \file openBMP.hpp
 * \author Tomasz Jakubczyk
 * \brief Plik z funkcją wczytującą BMP
 *
 */

#pragma once

#ifndef OPEN_BMP
#define OPEN_BMP

#include <iostream>
#include <cstdio>

struct Image
{
	unsigned int width, height;
	unsigned char * data;
};

Image* loadBMP_custom(const char * imagepath);

#endif // OPEN_BMP
