/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */

#include <vector>
#include <string>
#include "GlutImport.h"
#include <map>
#include <vector>
#include <iostream>
#include "jpeg_decoder.h"

/////////////////////////////////////////////////////////////
//
//	    PNG Texture Loader + alpha-transparency
//
/////////////////////////////////////////////////////////////

int IDerror; // texture used to show a texture error

int CreateTextureFromRGBA(unsigned char* data, int width, int height)
{
	GLuint t = 0;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return t;
}

int CreateTextureFromRGB(unsigned char* data, int width, int height)
{
	GLuint t = 0;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	return t;
}

// from PicoPNG LIB (see sources)
void loadFile(std::vector<unsigned char>& buffer, const std::string& filename);
int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

// flip image vertically
void VsymetryRGBAImage(std::vector<unsigned char>& data, int Width, int Height, int bytePerPix)
{
	for (int y = 0; y < Height / 2; y++)
	{
		int width = Width * bytePerPix;
		int pY = y * width;
		int sY = (Height - y - 1) * width;

		for (int x = 0; x < width; x++)
		{
			unsigned char t = data[pY + x];
			data[pY + x] = data[sY + x];
			data[sY + x] = t;
		}
	}
}

int LoadPNGintoTexture(const std::string& filename)
{
	std::vector<unsigned char> buffer, image;
	loadFile(buffer, filename);
	unsigned long w, h;
	int error = decodePNG(image, w, h, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size());

	int bytePerPix = (image.size() / (w * h) );
	VsymetryRGBAImage(image, w, h, bytePerPix);

	//if there's an error, display it
	if (error != 0)
	{
		std::cout << "error: " << error << std::endl;
		return IDerror;
	}

	//the pixels are now in the vector "image", use it as texture, draw it, ...

	int id = CreateTextureFromRGBA(image.data(), w, h);
	return id;
}




std::map<std::string, int> glTextKey;

int GetTextureIdFromPNG(std::string PNGFileName)
{
	// si jamais chargé, on charge le fichier image dans une texture opengl

	if (glTextKey.find(PNGFileName) == glTextKey.end()) // texture jamais chargée
		glTextKey[PNGFileName] = LoadPNGintoTexture(PNGFileName);

	// on retourn id de la texture associée à ce fichier
	return glTextKey[PNGFileName];
}

/////////////////////////////////////////////////////////////
//
//	    JPG Texture Loader
//
/////////////////////////////////////////////////////////////

int GetTextureIdFromJPG(std::string JPGFileName)
{
	size_t size;
	unsigned char* buf;
	FILE* f;


	f = fopen(JPGFileName.c_str(), "rb");
	if (!f) { std::cout << "Error opening the input file.\n";  return IDerror; }
	 
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	buf = (unsigned char*)malloc(size);
	if ( buf == NULL)
	{
		std::cout << "Memory error loading JPG\n";
		return IDerror;
	}
	fseek(f, 0, SEEK_SET);
	size_t read = fread(buf, 1, size, f);
	fclose(f);

	Jpeg::Decoder decoder(buf, size);
	int tt = decoder.GetResult();

	if (decoder.GetResult() != Jpeg::Decoder::OK)
	{
		std::cout << "Error decoding the input file\n";
		return IDerror;
	}

	std::cout << "JPG width  : " << decoder.GetWidth()  << std::endl;
	std::cout << "JPG height : " << decoder.GetHeight() << std::endl;
	
	if ( ! decoder.IsColor() )
	{
		std::cout << "Error - not an RGB image\n";
		return IDerror;
	}
	 

	unsigned char* buff = decoder.GetImage();
	size_t  len         = decoder.GetImageSize();


	int id = CreateTextureFromRGB(buff, decoder.GetWidth(), decoder.GetHeight());
	return id;
}
