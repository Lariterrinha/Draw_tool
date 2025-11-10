/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */
#pragma once

#include <string>
#include <iostream>

using namespace std;

enum class EventType { MouseMove, MouseDown, MouseUp, KeyDown, KeyUp };

struct Event
{
	EventType Type;
	string   info;
	int      x, y;

	bool isKeyboardEvent() const { return Type == EventType::KeyUp || Type == EventType::KeyDown;  }
	bool isMouseEvent() const    { return Type == EventType::MouseMove || Type == EventType::MouseDown || Type == EventType::MouseUp; }

	void print() const
	{
		string name[] = { "MouseMove", "MouseDown", "MouseUp", "KeyDown", "KeyUp" };
		cout << name[(int)Type] << " " << x << " " << y << " " << info << endl;
	}

	Event(EventType _Type, int _x, int _y, string _info) : Type(_Type), x(_x), y(_y), info(_info) {}
};

