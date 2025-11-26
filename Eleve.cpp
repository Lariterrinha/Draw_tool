/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */

#include <string>
#include <typeinfo>
#include "V2.h"
#include "Graphics.h"
#include "Event.h" 
#include "Model.h"
#include "Button.h"
#include "Tool.h"


using namespace std;

// ESC : close window

///////////////////////////////////////////////////////////////////////////////
//
//		setup screen

int main(int argc, char* argv[])
{
	std::cout << "Press ESC to abort" << endl;
	Graphics::initMainWindow("Pictor", V2(1200, 800), V2(200, 200));
}

void bntToolSegmentClick(Model& Data)   { Data.currentTool = make_shared<ToolSegment>(); }
void bntToolRectangleClick(Model& Data) { Data.currentTool = make_shared<ToolRectangle>(); }
void bntToolCircleClick(Model& Data) { Data.currentTool = make_shared<ToolCircle>(); }
void bntToolDelete(Model& Data) 
{
	// se ferramenta atual for ToolSelect e tiver seleção -> apagar só seleção
	if (Data.currentTool)
	{
		ToolSelect* ts = dynamic_cast<ToolSelect*>(Data.currentTool.get());
		if (ts && ts->getSelected())
		{
			ts->deleteSelection(Data);
			return;
		}
	}
	// senão limpa toda a cena
	Data.LObjets.clear();
}

void bntToolSelect(Model& Data) { Data.currentTool = make_shared<ToolSelect>(); }

void bntBringToFront(Model& Data)
{
	if (Data.currentTool)
	{
		ToolSelect* ts = dynamic_cast<ToolSelect*>(Data.currentTool.get());
		if (ts) ts->bringToFront(Data);
	}
}

void bntSendToBack(Model& Data)
{
	if (Data.currentTool)
	{
		ToolSelect* ts = dynamic_cast<ToolSelect*>(Data.currentTool.get());
		if (ts) ts->sendToBack(Data);
	}
}

// Etape 6: Drawing Options - cycle through colors, thickness, fill
void bntCycleBorderColor(Model& Data)
{
	static int idx = 0;
	Color colors[] = { Color::Red, Color::Green, Color::Blue, Color::Cyan, Color::Magenta, Color::Yellow, Color::Black };
	idx = (idx + 1) % 7;
	Data.drawingOptions.borderColor_ = colors[idx];
}

void bntCycleFillColor(Model& Data)
{
	static int idx = 0;
	Color colors[] = { Color::White, Color::Red, Color::Green, Color::Blue, Color::Cyan, Color::Magenta, Color::Yellow };
	idx = (idx + 1) % 7;
	Data.drawingOptions.interiorColor_ = colors[idx];
}

void bntCycleThickness(Model& Data)
{
	static int idx = 0;
	int thicknesses[] = { 1, 2, 3, 5 };
	idx = (idx + 1) % 4;
	Data.drawingOptions.thickness_ = thicknesses[idx];
}

void bntToggleFilled(Model& Data)
{
	Data.drawingOptions.isFilled_ = !Data.drawingOptions.isFilled_;
}
 
void bntToolPolygonClick(Model& Data)
{
	Data.currentTool = make_shared<ToolPolygonalLine>();
}

void initApp(Model& App)
{
	// choose default tool
	App.currentTool = make_shared<ToolSegment>();

	// create buttons
	int x = 0;
	int s = 80; // size

	// button Segment
	auto B1 = make_shared<Button>("Outil Segment", V2(x, 0), V2(s, s), "outil_segment.png", bntToolSegmentClick);
	App.LButtons.push_back(B1); 
	x += s;

	// button Rectangle
	auto B2 = make_shared<Button>("Outil Rectangle",V2(x, 0), V2(s, s), "outil_rectangle.png", bntToolRectangleClick);
	App.LButtons.push_back(B2);
	 x += s;

	// button Circle
	auto B3 = make_shared<Button>("Outil Circle", V2(x, 0), V2(s, s), "outil_ellipse.png", bntToolCircleClick);
	App.LButtons.push_back(B3);
	 x += s;

	// button Clean
	auto B4 = make_shared<Button>("Clean", V2(x, 0), V2(s, s), "outil_delete.png", bntToolDelete);
	App.LButtons.push_back(B4);
	 x += s;

	// button Select
	auto B5 = make_shared<Button>("Select", V2(x, 0), V2(s, s), "outil_move.png", bntToolSelect);
	App.LButtons.push_back(B5);
	 x += s;

	// button Bring to Front
	auto B6 = make_shared<Button>("Front", V2(x, 0), V2(s, s), "outil_up.png", bntBringToFront);
	App.LButtons.push_back(B6);
	 x += s;

	// button Send to Back
	auto B7 = make_shared<Button>("Back", V2(x, 0), V2(s, s), "outil_down.png", bntSendToBack);
	App.LButtons.push_back(B7);
	 x += s;

	// Etape 6: Drawing Options Buttons
	// button Border Color
	auto B8 = make_shared<Button>("BorderCol", V2(x, 0), V2(s, s), "outil_border_color.png", bntCycleBorderColor);
	App.LButtons.push_back(B8);
	 x += s;

	// button Fill Color
	auto B9 = make_shared<Button>("FillCol", V2(x, 0), V2(s, s), "outil_fill_color.png", bntCycleFillColor);
	App.LButtons.push_back(B9);
	 x += s;

	// button Thickness
	auto B10 = make_shared<Button>("Thick", V2(x, 0), V2(s, s), "outil_border_size.png", bntCycleThickness);
	App.LButtons.push_back(B10);
	 x += s;

	// button Toggle Filled
	auto B11 = make_shared<Button>("Filled", V2(x, 0), V2(s, s), "outil_no_fill.png", bntToggleFilled);
	App.LButtons.push_back(B11);
	 x += s;

	 auto B12 = make_shared<Button>("Polygon", V2(x, 0), V2(s, s),
		 "error.png", bntToolPolygonClick);
	 App.LButtons.push_back(B12);
	 x += s;



	// put two objets in the scene

	ObjAttr DrawOpt1 = ObjAttr(Color::Cyan, true, Color::Green, 6);
	auto newObj1 = make_shared<ObjRectangle>(DrawOpt1, V2(100, 100), V2(300, 200));
	App.LObjets.push_back(newObj1);

	ObjAttr DrawOpt2 = ObjAttr(Color::Red, true, Color::Blue, 5);
	auto newObj2 = make_shared<ObjRectangle>(DrawOpt2, V2(500, 300), V2(600, 600));
	App.LObjets.push_back(newObj2);
}

/////////////////////////////////////////////////////////////////////////
//
//		Event management

 
void processEvent(const Event& Ev, Model & Data)
{
	Ev.print(); // Debug

	// MouseMove event updates x,y coordinates
	if (Ev.Type == EventType::MouseMove ) Data.currentMousePos = V2(Ev.x, Ev.y);
	 

	// detect a mouse click on the tools icons

	V2 P = Data.currentMousePos;
	for (auto B : Data.LButtons)
		if (Ev.Type == EventType::MouseDown && P.isInside(B->getPos(),B->getSize()) )
		{
			B->manageEvent(Ev,Data);
			return;
		}


	// send event to the activated tool
	Data.currentTool->processEvent(Ev,Data);
	
	 
}


 
/////////////////////////////////////////////////////////////////////////
//
//     Drawing elements
 

void drawCursor(Graphics& G, const Model& D, const string s)
{
 

	V2 P = D.currentMousePos;
	int r = 7;
	const string a = "o";
	const string b = "O";
	
	Color c = Color::Black;
	G.drawLine(P + V2(r, 1), P + V2(-r, 1), c);
	G.drawLine(P + V2(r,-1), P + V2(-r,-1), c);
	G.drawLine(P + V2( 1,-r), P + V2( 1, r), c);
	G.drawLine(P + V2(-1,-r), P + V2(-1, r), c);

	Color cc = Color::White;
	G.drawLine(P - V2(r, 0), P + V2(r, 0), cc);
	G.drawLine(P - V2(0, r), P + V2(0, r), cc);

	G.drawStringFontMono(P + V2(20, 0), s, 20, 1, Color::Yellow);

	// Just to show the color of the current drawing options
	G.drawStringFontMono(P + V2(20, 0), b, 20, 1, D.drawingOptions.borderColor_);	// O
	if(D.drawingOptions.isFilled_)
		G.drawStringFontMono(P + V2(21, 3), a, 17, 1, D.drawingOptions.interiorColor_); // o
}
 
void drawCursor(Graphics& G, const Model& D)
{
	// the cursor is the name of the current tool
	string name = typeid(*D.currentTool).name(); 
	drawCursor(G, D, name.substr(5));
	
}

void drawApp(Graphics& G, const Model & D)
{
	// reset with a black background
	G.clearWindow(Color::Black);

	// draw all geometric objects
	for (auto& Obj : D.LObjets)
		Obj->draw(G);

	// draw the app menu
	for (auto& myButton : D.LButtons)
		myButton->draw(G);

	// draw current tool and interface (if active)
	D.currentTool->draw(G, D);

	// draw cursor
	drawCursor(G, D);
}

 




