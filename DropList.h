#pragma once

#include "Button.h"

class DropList
{
public:
	Rectq out;
	Rectq back;
	float height;

	int selected = -1;
	vector<Button> buttons;
	Font* font;
	Color textc;
	bool visible = false;
	bool expanded = false;
	
	void Expand();
	void Shrink();
	void Select(int s);
	void Set(float vx, float vy, float vw, float vh, Color bc, Color oc, Color tc, Font* vfont);
	void AddButton(wstring data);
	void Render();

	void Click(float x, float y);
};

