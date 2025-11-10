/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */
#pragma once
#include <string>
#include <vector>
#include "V2.h"
#include "color.h"

using namespace std;

class Graphics
{

public:

	// Window Management
	static void initMainWindow(string name, V2 ScreenSize, V2 WindowStartPos);
	
	V2   getWindowSize();
	void clearWindow(Color c);
	

	// Font
	void drawStringFontMono(V2 pos, string text, float fontSize = 20, float thickness = 3, Color c = Color::Black);
	void drawStringFontRoman(V2 pos, string text, float fontSize = 20, float thickness = 3, Color c = Color::Black);


	// draw a sprite associated to a filename
	// use angleDef for rotation
	void drawRectWithTexture(std::string filename, V2 pos, V2 size, float angleDeg = 0);

	// Draw Geometry
	void setPixel(V2 P, Color c);
	void drawLine(V2 P1, V2 P2, Color c, int thickness = 1);
	void drawPolygon(vector<V2>& PointList, Color c, bool fill = false, int thickness = 1);
	void drawRectangle(V2 P1, V2 Size, Color c, bool fill = false, int thickness = 1);
	void drawCircle(V2 C, float r, Color c, bool fill = false, int thickness = 1);


};