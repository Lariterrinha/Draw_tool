/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */
#pragma once

struct Color
{
	float R, G, B, A;   // 0<= R,G,B,A <= 1

	Color()                                   { R = G = B = A = 1;  }
	Color(float r, float g, float b)          { R = r; G = g; B = b; A = 1; }
	Color(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }

	const static Color Black, White, Red, Green, Blue, Magenta, Cyan, Yellow, Gray;  // write Color::Black
};

Color ColorFrom255(int r, int g, int b);
Color ColorFromHex(int hexCode);
