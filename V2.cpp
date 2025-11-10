/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */

#include <iostream>
#include "V2.h"


// given 2 points, returns lowest and leftmost point + the dimensions W/H
void getPLH(V2 P1, V2 P2, V2& P, V2& size)
{
	int W = abs(P1.x - P2.x);
	int H = abs(P1.y - P2.y);
	size = V2(W, H);

	int minX = std::min(P1.x, P2.x);
	int minY = std::min(P1.y, P2.y);
	P = V2(minX, minY);
}

// constructors

V2::V2(int    _x, int     _y)  { x = _x; y = _y; }

V2::V2(double _x, double  _y)  { x = (int)_x; y = (int)_y; }

V2::V2(float  _x, float   _y) { x = (int)_x; y = (int)_y; }

 
// Is the current point inside the rectangle defined by pos + W/H
bool V2::isInside(V2 pos, V2 WH)
{
	int dx = this->x - pos.x;
	int dy = this->y - pos.y;

	return ( (dx >= 0) && (dx < WH.x) && (dy >= 0) && (dy < WH.y) );
}


// norms

double V2::norm() { return sqrt(x * x + y * y); }
 

// float comparison

bool operator == (const V2 & a, const V2 & b)
{
	V2 t = a - b;
	float epsilon = 0.001f;
	return t.norm() < epsilon;
}

// definition of standard operators

V2 operator + (const V2 & a, const V2 & b) {  return V2(a.x + b.x, a.y + b.y); }
V2 operator - (const V2 & a, const V2 & b) {  return V2(a.x - b.x, a.y - b.y); }
V2 operator * (float      a, const V2 & b) {  return V2(a   * b.x, a   * b.y); }
V2 operator * (const V2 & a, float      b) {  return V2(a.x * b  , a.y * b);   }
V2 operator / (const V2 & a, float      b) {  return V2(a.x / b  , a.y / b);   }
V2 operator - (const V2 & a)               {  return V2( -a.x, -a.y ); }

 

// print 

std::ostream & operator << (std::ostream & os, const V2 & t)
{
	os << "(" << t.x << "," << t.y << ")";
	return os;
}