/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */
#pragma once

#include <string>
#include "V2.h"
#include "Graphics.h"
#include "Event.h"
#include "Model.h"
#include <functional>

using namespace std;

// button component in the GUI


class Button
{
	string imageFile_;
	string myName_;
	V2 pos_;
	V2 size_;
	function<void(Model&)> storedFunction_; // when the button is clicked, call this function

 

public:

	V2 getPos()  { return pos_;  }
	V2 getSize() { return size_; }

	Button(string myName, V2 pos, V2 size, string imageFile, function<void(Model&)> callBack) :
		myName_(myName), pos_(pos), size_(size), imageFile_(imageFile), storedFunction_(callBack) {  }

	void manageEvent(const Event& Ev, Model& Ap)
	{
		if (Ev.Type == EventType::MouseDown)
		{
			cout << ">> Clic sur le bouton " << myName_ << endl;
			// muda o nome no mouse (?)

			storedFunction_(Ap);
		}
	}

	void draw(Graphics & G)
	{
		G.drawRectWithTexture(imageFile_, pos_, size_);
		G.drawRectangle(pos_, size_, Color::Gray, false,2);
		G.drawRectangle(pos_ + V2(2,2), size_-V2(4,4), Color::Black, false,2);
	}

	
};