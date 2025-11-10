/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */
#pragma once
#include "Color.h"

// store color information for objects

struct ObjAttr
{
public:
	Color borderColor_;
	Color interiorColor_;
	int   thickness_;
	bool  isFilled_;

	ObjAttr()
	{
		borderColor_   = Color::Cyan;
		interiorColor_ = Color::Yellow;
		isFilled_      = true;
		thickness_     = 1;
	}

	ObjAttr(Color borderColor, bool isFilled, Color interiorColor, int thickness)
	{
		borderColor_   = borderColor;
		isFilled_      = isFilled;
		interiorColor_ = interiorColor;
		thickness_     = thickness;
	}
};