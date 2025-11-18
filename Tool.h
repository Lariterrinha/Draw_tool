/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */
#pragma once

#include <string>
#include "Event.h"
#include "Model.h"
#include "ObjGeom.h"

using namespace std;

enum State { WAIT, INTERACT  };

////////////////////////////////////////////////////////////////////

class Tool
{
protected :
	int currentState;

public:
	Tool() {   currentState = State::WAIT; }
	virtual void processEvent(const Event& E, Model & Data) {}

	virtual void draw(Graphics& G,const  Model& Data) {}
};

////////////////////////////////////////////////////////////////////


class ToolSegment : public Tool
{
	V2 Pstart;
public : 

	ToolSegment() : Tool() {}
	void processEvent(const Event& E, Model& Data) override
	{
		if (E.Type == EventType::MouseDown && E.info == "0") // left mouse button pressed
		{
			if (currentState == State::WAIT)
			{
				// interactive drawing
				Pstart = Data.currentMousePos;
				currentState = State::INTERACT;  
				return;
			}
		}

		if (E.Type == EventType::MouseUp && E.info == "0") // left mouse button released
		{
			if (currentState == State::INTERACT)
			{
				// add object in the scene
				V2 P2 = Data.currentMousePos;
				auto newObj = make_shared<ObjSegment>(Data.drawingOptions, Pstart, P2);
				Data.LObjets.push_back(newObj);
				currentState = State::WAIT;
				return;
			}
		}
	}


	void draw(Graphics& G, const Model& Data) override
	{
		if (currentState == State::INTERACT)
  		  G.drawLine(Pstart, Data.currentMousePos, Data.drawingOptions.borderColor_, Data.drawingOptions.thickness_);
	}

};


////////////////////////////////////////////////////////////////////


class ToolRectangle : public Tool
{
protected:
	V2 Pstart;
public:

	ToolRectangle() : Tool() {}
	void processEvent(const Event& E, Model& Data) override
	{
		if (E.Type == EventType::MouseDown && E.info == "0") // left mouse button pressed
		{
			if (currentState == State::WAIT)
			{
				// interactive drawing
				Pstart = Data.currentMousePos;
				currentState = State::INTERACT;
				return;
			}
		}

		if (E.Type == EventType::MouseUp && E.info == "0") // left mouse button released
		{
			if (currentState == State::INTERACT)
			{
				// add object in the scene
				V2 P2 = Data.currentMousePos;
				auto newObj = make_shared<ObjRectangle>(Data.drawingOptions, Pstart, P2);
				Data.LObjets.push_back(newObj);
				currentState = State::WAIT;
				return;
			}
		}
	}

	void draw(Graphics& G, const Model& Data) override
	{
		if (currentState == State::INTERACT){

			// pre-view 
			V2 P; V2 size;
			getPLH(Pstart, Data.currentMousePos, P, size); // normalize

			// retangulo 
			G.drawRectangle(P, size, Data.drawingOptions.borderColor_, true, Data.drawingOptions.thickness_);

			//G.drawRectangle(Pstart, Data.currentMousePos, Data.drawingOptions.borderColor_, Data);
		}
	}
};

class ToolCircle : public Tool
{
protected:
	V2 Pstart;
public:

	ToolCircle() : Tool() {}
	void processEvent(const Event& E, Model& Data) override
	{
		if (E.Type == EventType::MouseDown && E.info == "0") // left mouse button pressed
		{
			if (currentState == State::WAIT)
			{
				// interactive drawing
				Pstart = Data.currentMousePos;
				currentState = State::INTERACT;
				return;
			}
		}

		if (E.Type == EventType::MouseUp && E.info == "0") // left mouse button released
		{
			if (currentState == State::INTERACT)
			{
				// add object in the scene
				V2 P2 = Data.currentMousePos;
				auto newObj = make_shared<ObjCircle>(Data.drawingOptions, Pstart, P2);
				Data.LObjets.push_back(newObj);
				currentState = State::WAIT;
				return;
			}
		}
	}

	void draw(Graphics& G, const Model& Data) override
	{
		if (currentState == State::INTERACT) {

			// pre-view  
			int r;

			V2 diff = Data.currentMousePos - Pstart;
			r = (int)diff.norm();
			
			// circulo 
			G.drawCircle(Pstart,r , Data.drawingOptions.borderColor_, true, Data.drawingOptions.thickness_);

		}
	}
};

class ToolSelect : public Tool
{
protected:
	std::shared_ptr<ObjGeom> selectedObj_;

public:

	ToolSelect() : Tool(), selectedObj_(nullptr) {}

	// retorna seleção
	std::shared_ptr<ObjGeom> getSelected() const { return selectedObj_; }

	// remove a seleção da cena (procura por ponteiro)
	void deleteSelection(Model& Data)
	{
		if (!selectedObj_) return;
		for (auto it = Data.LObjets.begin(); it != Data.LObjets.end(); ++it)
		{
			if (it->get() == selectedObj_.get())
			{
				Data.LObjets.erase(it);
				selectedObj_.reset();
				return;
			}
		}
	}
	// move object to front (end of list = drawn last = on top)
	void bringToFront(Model& Data)
	{
		if (!selectedObj_) return;
		// find and move to end
		for (auto it = Data.LObjets.begin(); it != Data.LObjets.end(); ++it)
		{
			if (it->get() == selectedObj_.get())
			{
				auto obj = *it;
				Data.LObjets.erase(it);
				Data.LObjets.push_back(obj);
				return;
			}
		}
	}

	// move object to back (start of list = drawn first = behind)
	void sendToBack(Model& Data)
	{
		if (!selectedObj_) return;
		// find and move to beginning
		for (auto it = Data.LObjets.begin(); it != Data.LObjets.end(); ++it)
		{
			if (it->get() == selectedObj_.get())
			{
				auto obj = *it;
				Data.LObjets.erase(it);
				Data.LObjets.insert(Data.LObjets.begin(), obj);
				return;
			}
		}
	}

	void processEvent(const Event& E, Model& Data) override
	{
		// seleção no MouseUp (botão esquerdo)
		if (E.Type == EventType::MouseUp && E.info == "0")
		{
			std::shared_ptr<ObjGeom> found = nullptr;
			// procura do topo para baixo (up to down)
			for (int i = (int)Data.LObjets.size() - 1; i >= 0; --i)
			{
				auto &obj = Data.LObjets[i];
				if (!obj) continue;

				// usa contains polimorfico
				if (obj->contains(Data.currentMousePos)) { found = obj; break; }
			}

			// toggle: se clicou no mesmo objeto -> desseleciona
			if (found && selectedObj_ && found.get() == selectedObj_.get())
				selectedObj_.reset();
			else
				selectedObj_ = found;
		}
	}

	void draw(Graphics& G, const Model& Data) override
	{
		if (!selectedObj_) return;

		// usa getBoundingBox polimórfico
		V2 P, size;
		selectedObj_->getBoundingBox(P, size);
		// desenha moldura magenta um pouco maior
		G.drawRectangle(P - V2(4,4), size + V2(8,8), Color::Magenta, false, 4);
	}
};

