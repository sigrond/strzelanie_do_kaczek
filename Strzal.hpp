/** \file Strzal.hpp
 * \author Tomasz Jakubczyk
 * \brief Plik nagłówkowy funkcji wyliczającej trafienie
 *
 */

#pragma once

#include <cmath>
#include <iostream>

using namespace std;

bool Strzal(float x,float y,float z,float phi,float theta, float* kaczka)
{
	float Vx,Vy,Vz;
	//float delta=2.29244f;//+/- r kaczki=2.5 , szerokość kaczki = 0.2 arcsin(0,1/2,5)
	float delta=30;
	//theta=-theta;
	theta+=30;
	while(phi>360+delta || phi<0+delta)
	{
		if(phi>360+delta)
			phi-=360;
		if(phi<0+delta)
			phi+=360;
	}
	while(theta>360+delta || theta<0+delta)
	{
		if(theta>360+delta)
			theta-=360;
		if(theta<0+delta)
			theta+=360;
	}
	Vx=kaczka[0]-x;
	Vy=kaczka[1]-y;
	Vz=kaczka[2]-z;
	float a=atan(Vz/Vx)*180/3.1415f;//kąt strzału poziomo
	float b=atan(Vx/Vy)*180/3.1415f;//kąt strzału pionowo
	while(a>360+delta || a<0+delta)
	{
		if(a>360+delta)
			a-=360;
		if(a<0+delta)
			a+=360;
	}
	while(b>360+delta || b<0+delta)
	{
		if(b>360+delta)
			b-=360;
		if(b<0+delta)
			b+=360;
	}
	cout<<"phi: "<<phi<<", theta: "<<theta<<endl;
	cout<<"a: "<<a<<", b: "<<b<<endl;
	if((a+delta>phi && a-delta<phi) && (b+delta>theta && b-delta<theta))
	{
		cout<<"Trafiona!"<<endl;
		return true;
	}
	return false;
}
