#include "ToolBox.h"

void ToolBox::Set(float dx, float dy, float dw, float dh, Color q, Font* font)
{
	this->back.Set(dx, dy, dw, dh, q);
	this->font = font;	
	this->title.setFont(this->font);
	this->title.setPosition(dx + 0.01, dy - 0.015);
	this->title.setData(L"ToolBox");
	this->title.setColor(Color(0.9f, 0.9f, 0.9f));
	this->l.Set(dx, dy - 0.04, dx + dw, dy - 0.04, Color(0.44313725490, 0.37647058824, 0.90980392157));
}

void ToolBox::SetWinSize(float x, float y)
{
	this->winsizex = x;
	this->winsizey = y;
}

void ToolBox::Render()
{
	if (this->visible)
	{
		this->back.Render();
		this->l.Render();
		this->title.Render();
		for (auto& b : this->tools)
			b.Render();
	}
}

void ToolBox::SetButtons()
{
	this->NewButton(L"Button");
	this->NewButton(L"Entry");
	this->NewButton(L"Label");
	this->NewButton(L"CheckButton");
	this->NewButton(L"Menu");
	this->NewButton(L"ComboBox");
	this->NewButton(L"ListBox");
	this->NewButton(L"MenuButton");
	this->NewButton(L"Canvas");
	this->NewButton(L"Scale");
	this->NewButton(L"ScrollBar");
	this->NewButton(L"TopLevel");
	this->NewButton(L"Frame");
	this->NewButton(L"RadioButton");
}

void ToolBox::NewButton(wstring text)
{
	Button button;
	button.Set(this->back.x1 + 0.05, this->back.y1 - 0.1 - this->tools.size()*this->font->font->glcharsize*4, this->back.x2 - this->back.x1 - 0.1, this->font->font->glcharsize*2.5, Color(0.25, 0.25, 0.25), this->font, Left);
	button.text.setColor(Color(0.8, 0.8, 0.8));
	button.text.setData(text);
	button.text.setPosition(button.back.x1 + 0.02, button.back.y1 - 0.013);
	this->tools.push_back(button);
}

int ToolBox::MouseDown(float px, float py)
{
	if (px > this->back.x1 + 0.05 && px < this->back.x2 - 0.05 && py < this->back.y1 - 0.1)
	{
		float start =  this->back.y1 - 0.1 - py;
		return start / (this->font->font->glcharsize * 4);
	}
	return -1;
}
