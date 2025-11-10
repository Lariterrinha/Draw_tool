/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */
#include <string>
#include "GlutImport.h"
#include "Graphics.h"
#include "V2.h"
#include "color.h"
#include "Event.h"
#include "Model.h"


///////////////////////////////////////////////
//
//    gestion des CallBack

 
Model Data;

// dans eleves.cpp

void processEvent(const Event& V, Model & AppData);

void drawApp(Graphics& G, const Model & AppData);

namespace GL
{
	void Show();
	void InitWindow(string name, V2 ScreenSize, V2 WindowStartPos);
	void AskScreenRedraw();
};

void myglEvent(Event& V)
{
	
	processEvent(V, Data);
	GL::AskScreenRedraw();
}

 
void GLRender()
{
	Graphics G;
    drawApp(G, Data);
	GL::Show();
}

 

void MainWindowInit(string name, V2 ScreenSize, V2 WindowStartPos)
{
	GL::InitWindow(name, ScreenSize, WindowStartPos);
}



/////////////////////////////////////////////////////////////
//
//
//  appels au noyau OpenGl

V2 Wsize;
extern int IDerror;
int LoadPNGintoTexture(const std::string& filename);

namespace GL
{
	


	/////////////////////////////////////////////////////////
	//
	//     events
	//

	// détecte un changement de taille de la fenêtre et remet la taille initiale

	V2 GetWindowSize() { return Wsize; }

	void reshape(int w, int h)
	{
		glutReshapeWindow(Wsize.x, Wsize.y);
	}

	void KeyPressed(unsigned char key, int x, int y)
	{
		if (key == 0x1b) exit(0);   // ESC => ABORT
		myglEvent( Event(EventType::KeyDown, -1, -1, std::string(1,key)));
	}

	void  keyUp(unsigned char key, int x, int y)
	{
		myglEvent( Event(EventType::KeyUp, -1, -1, std::string(1, key)));
	}

	string  decode(int KeyCode)
	{
		switch (KeyCode)
		{
		case GLUT_KEY_LEFT:		return "LEFT";
		case GLUT_KEY_RIGHT:	return "RIGHT";
		case GLUT_KEY_DOWN:		return "DOWN";
		case GLUT_KEY_UP:		return "UP";
		case GLUT_KEY_F1:		return "F1";
		case GLUT_KEY_F2:		return "F2";
		case GLUT_KEY_F3:		return "F3";
		case GLUT_KEY_F4:		return "F4";
		case GLUT_KEY_F5:		return "F5";
		case GLUT_KEY_F6:		return "F6";
		case GLUT_KEY_F7:		return "F7";
		case GLUT_KEY_F8:		return "F8";
		case GLUT_KEY_F9:		return "F9";
		case GLUT_KEY_F10:		return "F10";
		case GLUT_KEY_F11:		return "F11";
		case GLUT_KEY_F12:		return "F12";

		default:
			return string("Unknown special key - code : ") + to_string(KeyCode);
		}
	}

	void keySpecial(int key, int x, int y)
	{
		myglEvent( Event(EventType::KeyDown, -1, -1, decode(key) ));
	}

	void keySpecialUp(int key, int x, int y)
	{
		myglEvent(Event(EventType::KeyUp, -1, -1, decode(key)));
	}


	void mouse(int button, int state, int x, int y)
	{
		if (state == GLUT_DOWN)
			myglEvent( Event(EventType::MouseDown, -1, -1, to_string(button) ));

		if (state == GLUT_UP)
			myglEvent( Event(EventType::MouseUp, -1, -1, to_string(button)));
			 
	}

	void mouseMove(int x, int y)
	{
		int yy = Wsize.y - y;
		myglEvent( Event(EventType::MouseMove, x, yy, ""));
	}

	///////////////////////////////////////////////////////

	// openGL commands

	void AskScreenRedraw() { glutPostRedisplay(); }


	void Show()
	{
		glFlush();          // single buffer
		glutSwapBuffers();  // double buffer
	}


	void InitCamera(V2 ScreenSize)
	{
		
		// if (HideCursor) glutSetCursor(GLUT_CURSOR_NONE);

		// Matrice de projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, ScreenSize.x, 0, ScreenSize.y, -1, 10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);



		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(1);

		// glEnable(GL_POLYGON_SMOOTH); à éviter rendu erroné



		// callbacks
		glutReshapeFunc(reshape);

		glutKeyboardFunc(KeyPressed);
		glutKeyboardUpFunc(keyUp);
		glutSpecialFunc(keySpecial);
		glutSpecialUpFunc(keySpecialUp);

		glutMotionFunc(mouseMove);
		glutPassiveMotionFunc(mouseMove);
		glutMouseFunc(mouse);


		glutDisplayFunc(GLRender);        // fonction appelée lors d'un repaint
		//glutTimerFunc(100, fnt, 0);         
		glutMainLoop();
	}
	 

	void PrintOpenGLVersion()
	{
		/*
		const GLubyte* version = glGetString(GL_VERSION);
		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* vendor   = glGetString(GL_VENDOR);

		std::cout << "OpenGL version : " << version << std::endl;
		std::cout << "Renderer       : " << renderer << std::endl;
		std::cout << "Vendor         : " << vendor << std::endl;
		*/
	}

	void InitWindow(string name, V2 ScreenSize, V2 WindowStartPos)
	{
		int   argcc = 3;
		char  arg0[] = "PICTOR 2000";
		char  arg1[] = "arg";
		char  arg2[] = "another arg";
		char* argc[] = { arg0, arg1, arg2, NULL };
		glutInit(&argcc, argc);

		Wsize = ScreenSize;

		glutInitWindowPosition(WindowStartPos.x, WindowStartPos.y);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowSize(ScreenSize.x, ScreenSize.y);
		glutCreateWindow(name.c_str());

		glutSetCursor(GLUT_CURSOR_NONE);

		IDerror = LoadPNGintoTexture("error.png");

		PrintOpenGLVersion();

		GL::InitCamera(ScreenSize);
 
		// plus aucun code exécutée après cette ligne
	}
};

 
