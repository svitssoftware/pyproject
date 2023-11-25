#pragma once
#include "GL.h"

#include <ft2build.h>
#include <freetype/freetype.h>

#include <vector>
#include <string>
using namespace std;

struct Char
{
	GLuint texName;
	float sizex, sizey, advance, xbearing, ybearing;
};

struct FontArray
{
	vector<Char> chars;
	int charsize;
	float glcharsize;
};

class Font
{
public:
	FT_Library  library;
	FT_Face     face;
	void LoadFromFile(string path, int xwin, int ywin);
	void Load(FontArray*& fa, int size, string path, int xwin, int ywin);
	FontArray* font;
	FontArray* sfont;
	float factor = 1.0f;
};

