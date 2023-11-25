#pragma once

#include "GL.h"
#include "Primitives.h"
#include "Text.h"

class TextBox
{
public:
	Text data;
	Rectq back;
	Rectq out;
	Line Cursor;
	int pos;

	bool selected = false;

	void Set(float x, float y, float w, float h, Color bc, Color oc, Color tc, Font* font);
	void Add(wchar_t key);
	void KeyPressed(wchar_t key);
	void BackSpace();
	void Render();
};