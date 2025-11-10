#include "Graphics.h"
#include "GlutImport.h"
#include <algorithm>


extern V2 Wsize;
V2   Graphics::getWindowSize()
{
	return Wsize;
}

/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */


/////////////////////////////////////////////////////////////
//
//	    RectWithTexture
//
/////////////////////////////////////////////////////////////

int GetTextureIdFromPNG(std::string PNGFileName);
int GetTextureIdFromJPG(std::string JPGFileName);

string GetExtension(string filename)
{
	return filename.substr(filename.size() - 4);
}

#include <cctype>
static std::string ToLower(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return (char)std::tolower(c); });
	return s;
}
static std::string GetExtSafe(const std::string& f) {
	auto s = ToLower(f);
	if (s.size() >= 5 && s.rfind(".jpeg") == s.size() - 5) return ".jpeg";
	if (s.size() >= 4) return s.substr(s.size() - 4); // .jpg / .png
	return "";
}

void Graphics::drawRectWithTexture(std::string JPGPNGFileName, V2 pos, V2 size, float angleDeg)
{
	// --- choix texture
	int idTexture = 0;
	auto ext = GetExtSafe(JPGPNGFileName);

	if (ext == ".jpg" || ext == ".jpeg")      idTexture = GetTextureIdFromJPG(JPGPNGFileName);
	else if (ext == ".png")                   idTexture = GetTextureIdFromPNG(JPGPNGFileName);
	else                                      idTexture = GetTextureIdFromPNG("error.png");

	// --- état rendu
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	#define GL_CLAMP_TO_EDGE 0x812F

	if (idTexture > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, (GLuint)idTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Facultatif mais utile contre les franges :
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(255, 255, 255, 255);          // ne pas assombrir la texture
	}
	else 
	{
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor4ub(255, 0, 255, 255);            // magenta “error”
	}

	float w = size.x, h = size.y;
	const double MPI = 3.14159265358979323846;

	// centre du quad
	float cx = pos.x + w * 0.5f;
	float cy = pos.y + h * 0.5f;

	glPushMatrix();
	glTranslatef(cx, cy, 0.0f);
	glRotatef(angleDeg, 0.0f, 0.0f, 1.0f);

	// dessiner autour de l'origine (plus simple pour la rotation)
	float x0 = -w * 0.5f, y0 = -h * 0.5f;
	float x1 = w * 0.5f, y1 = h * 0.5f;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y0, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, y1, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y1, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, y0, 0.0f);
	glEnd();

	glPopMatrix();

	// Nettoyage léger (optionnel)
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glColor4ub(255, 255, 255, 255);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


/////////////////////////////////////////////////////////////
//
//	    Geometry
//
/////////////////////////////////////////////////////////////

void Graphics::clearWindow(Color c) 
{
	glClearColor(c.R, c.G, c.B, c.A);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::setPixel(V2 P, Color c) 
{
	glColor4d(c.R, c.G, c.B, c.A);
	glBegin(GL_POINTS);
	glVertex2i(P.x, P.y); //Set pixel coordinates 
	glEnd();
	//glFlush(); //Render pixel
}


void Graphics::drawRectangle(V2 P1, V2 Size, Color c, bool fill, int thickness) 
{
	glDisable(GL_TEXTURE_2D);                 // pas de texture

	glLineWidth(thickness);
	glColor4d(c.R, c.G, c.B, c.A);
	glDisable(GL_TEXTURE_2D);

	if (fill)   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else    	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRecti((int)P1.x, (int)P1.y, (int)(P1.x + Size.x), (int)(P1.y + Size.y));

}

void Graphics::drawCircle(V2 C, float r, Color c, bool fill, int thickness)
{
	glLineWidth(thickness);

	vector<V2> LPoints;

	int lineAmount = r / 4; //  nb of triangles used to draw circle
	if (lineAmount < 20) lineAmount = 20;

	const double PI = 3.14159265358;

	//GLfloat radius = 0.8f; //radius
	double step = 2 * PI / lineAmount;

	for (int i = 0; i <= lineAmount; i++)
		LPoints.push_back(V2(C.x + r * cos(i * step), C.y + r * sin(i * step)));

	Graphics::drawPolygon(LPoints, c, fill);
}

void Graphics::drawLine(V2 P1, V2 P2, Color c, int thickness)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth((GLfloat)thickness);
	glColor4d(c.R, c.G, c.B, c.A);

	glBegin(GL_LINES);
	glVertex2f(P1.x, P1.y);
	glVertex2f(P2.x, P2.y);
	glEnd();

	glLineWidth(1.0f);
	glDisable(GL_BLEND);
}

void Graphics::drawPolygon(vector<V2>& PointList, Color c, bool fill, int thickness)
{
	glDisable(GL_TEXTURE_2D);
	glColor4d(c.R, c.G, c.B, c.A);
	glLineWidth(thickness);

	if (fill) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	 

	glBegin(GL_POLYGON);
	for (V2 P : PointList)
		glVertex2f(P.x, P.y);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // reset d’hygiène
}

/////////////////////////////////////////////////////////////
//
//	    Font
//
/////////////////////////////////////////////////////////////


void DrawString(V2 pos, string text, float fontSize, float thickness, Color c, bool FontMono)
{
	glColor4f(c.R, c.G, c.B, c.A);


	// EPAISSEUR de la font
	glLineWidth(thickness);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glScalef(1 / 152.38, 1 / 152.38, 1 / 152.38);
	glScalef(fontSize, fontSize, fontSize);
	const char* cc = text.c_str();
	for (char* p = (char*)cc; *p; p++)
	{
		if (FontMono) glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
		else          glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	}
	glPopMatrix();
	glLineWidth(1);
}


void Graphics::drawStringFontMono(V2 pos, string text, float fontSize, float thickness, Color c) 
{
	glDisable(GL_TEXTURE_2D);
	DrawString(pos, text, fontSize, thickness, c, true);
}

void Graphics::drawStringFontRoman(V2 pos, string text, float fontSize, float thickness, Color c) 
{
	glDisable(GL_TEXTURE_2D);
	DrawString(pos, text, fontSize, thickness, c, false);
}


//////////////////////////////////////////////

void MainWindowInit(string name, V2 WindowSize, V2 WindowStartPos);

void Graphics::initMainWindow(string name, V2 WindowSize, V2 WindowStartPos)
{
	MainWindowInit(name, WindowSize, WindowStartPos);
}