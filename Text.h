#pragma once

#include "GL.h"
#include "Font.h"

class Text
{
public:
	Text();
	~Text();
	Color c;
	Font* font;
	FontArray* fa;
	float x, y;
	float maxsizex = 0;
	int orientation = 0;
	float scale = 1;
public:
	wstring data;
	void setColor(Color vc);
	void setPosition(float vx, float vy);
	void setFont(Font* vfont);
	void setScale(float vscale);
public:
	void setData(wstring vdata);
	void addData(wstring vdata)
	{
		for (wchar_t& each : vdata)
			addData(each);
	}
	void addData(wchar_t vcdata);
	void removeData(int start, int count);
public:
	void Render();
private:
};