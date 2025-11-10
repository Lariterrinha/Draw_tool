/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */
#pragma once
#include <ostream>

struct V2 
{
	int x, y;

	V2(int    _x, int     _y);
	V2(double _x, double  _y);
	V2(float  _x, float   _y);
	V2() : x(0), y(0) {} 

	double norm();

	bool isInside(V2 pos, V2 WH); // check if the point is inside the rectangle pos/WH
};

// comparaison

bool operator == (const V2 & a, const V2 & b);

// redefinition of standard operator

V2 operator + (const V2 & a, const V2 & b);
V2 operator - (const V2 & a, const V2 & b);
V2 operator * (float      a, const V2 & b);
V2 operator * (const V2 & a, float      b);
V2 operator / (const V2 & a, float      b);
V2 operator - (const V2 & a);  // - unaire
 

// affichage 
std::ostream & operator << (std::ostream & os, const V2 & t);

// given 2 points, returns lowest and leftmost point + the dimensions W/H
void getPLH(V2 P1, V2 P2, V2& P, V2& size);