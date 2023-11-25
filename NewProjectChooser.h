#pragma once
#include "GL.h"
#include "Primitives.h"
#include "TextBox.h"
#include "Text.h"
#include "Button.h"
#include "DropList.h"
#include "ImageButton.h"
#include <Shobjidl.h>

class NewProjectChooser
{
public:
	Rectq out;
	Rectq back;

	Text title;

	Text tname;
	TextBox name;

	Text tdirectory;
	TextBox directory;
	Image bdirectory;

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
