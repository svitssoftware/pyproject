#pragma once
#include "GL.h"
#include "Primitives.h"
#include "TextBox.h"
#include "Text.h"
#include "Button.h"
#include "DropList.h";

class NewFileChooser
{
public:
	Rectq out;
	Rectq back;

	Text title;

	Text tname;
	TextBox name;

	Text ttype;
	DropList type;

	Button ok;
	Button cancel;
    
	bool active = false;

	void Set(float x, float y, float w, float h, Color bc, Color oc, Color tc, Font* font);
	void Render();

	int MouseClick(float x, float y);
	void KeyPressed(wchar_t key);
};