#pragma once
#include "Text.h"
#include "Button.h"

class ToolBox
{
public:
	Rectq back;
	Text title;
	Line l;
	vector<Button> tools;
	Font* font;
	float winsizex, winsizey;

	bool visible = false;

	void Set(float dx, float dy, float dw, float dh, Color q, Font* font);
	void SetWinSize(float x, float y);
	void Render();
	void SetButtons();
	void NewButton(wstring text);

	int MouseDown(float px, float py);
};

