#include "DropList.h"
#include <iostream>

void DropList::Expand()
{
	this->expanded = true;

	this->back.y2 = this->back.y1 - this->buttons.size() * 0.06 - 0.01;
	this->out.y2 = this->back.y2;

	for (auto& b : this->buttons)
		b.visible = true;
}

void DropList::Shrink()
{
	this->expanded = false;

	this->back.y2 = this->back.y1 - 0.06 - 0.01;
	this->out.y2 = this->back.y2;

	for (auto& b : this->buttons)
		b.visible = false;

	this->buttons[this->selected].visible = true;
}

void DropList::Select(int s)
{
	if (s != this->selected)
	{
		this->selected = s; 
		this->buttons[s].visible = true;
		this->buttons[s].back.y1 = this->back.y1 - 0.02;
		this->buttons[s].back.y2 = this->back.y1 - 0.06;
		this->buttons[s].text.setPosition(this->buttons[s].text.x, this->buttons[s].back.y1);

		float yposch = this->back.y1 - 0.08;
		for (int i = 0; i < this->buttons.size(); i++)
		{
			if (i != s)
			{
				this->buttons[i].back.y1 = yposch;
				this->buttons[i].back.y2 = yposch - 0.06;
				this->buttons[i].text.setPosition(this->buttons[i].text.x, this->buttons[i].back.y1);
				yposch = this->buttons[i].back.y2;
			}
		}
	}
}

void DropList::Set(float vx, float vy, float vw, float vh, Color bc, Color oc, Color tc, Font* vfont)
{
	this->back.Set(vx, vy, vw, vh, bc);
	this->out.Set(vx, vy, vw, vh, oc);
	this->textc = tc;
	this->font = vfont;
	this->height = vh;
}

void DropList::AddButton(wstring data)
{
	Button b;
	b.Set(this->back.x1 + 0.01, this->back.y1 - this->buttons.size()*0.06 - 0.01, this->back.x2 - this->back.x1 - 0.02, 0.05, this->back.qolor, this->font, Left);
	b.SetText(data, this->textc);
	b.visible = false;
	this->buttons.push_back(b);
}

void DropList::Render()
{
	if (this->visible)
	{
		this->back.Render();
		for (auto& b : this->buttons)
			b.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->out.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void DropList::Click(float x, float y)
{
	if (this->expanded)
	{
		for (int i = 0; i < this->buttons.size(); i++)
		{
			if (this->buttons[i].back.y1 > y && this->buttons[i].back.y2 < y)
			{
				this->Select(i);
			}
		}
		this->Shrink();
	}
	else
	{
		this->Expand();
	}
}