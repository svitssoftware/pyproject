#pragma once
#include "Components.h"
#include "Primitives.h"

class Button
{
public:
	void Set(float x, float y, float width, float height, Color c, Font* vfont, Anchor anchor, float z = 0);
	void SetOutline(Color oc);
	void SetText(wstring text, Color tc, bool autosize = false);
	void SizeToText();
	void Render();

	Text text;
	Rectq back;
	Rectq out;

	bool outlined = false;
	bool visible = true;
	Anchor an;

	int ID;
	int parentID = 0;
};

