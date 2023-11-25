#include "NewFileChooser.h"

void NewFileChooser::Set(float x, float y, float w, float h, Color bc, Color oc, Color tc, Font* font)
{
	this->back.Set(x, y, w, h, bc);
	this->out.Set(x, y, w, h, oc);

	this->title.setFont(font);
	this->title.setColor(oc);
	this->title.setPosition(x + 0.05, y - 0.1);
	this->title.setData(L"New File");
	this->title.setScale(2);

	this->tname.setFont(font);
	this->tname.setColor(tc);
	this->tname.setPosition(x + 0.1, y - 0.25);
	this->tname.setData(L"Name of file:");

	this->name.Set(x + 0.1, y - 0.3, 0.5, 0.06, bc, tc, tc, font);

	this->ok.Set(x + w - 0.2, y - h + 0.1, 0.15, 0.05, bc, font, Center);
	this->ok.SetOutline(tc);
	this->ok.SetText(L"Ok", tc);

	this->cancel.Set(x + 2 - 0.4, y - h + 0.1, 0.15, 0.05, bc, font, Center);
	this->cancel.SetOutline(tc);
	this->cancel.SetText(L"Cancel", tc);

	this->ttype.setFont(font);
	this->ttype.setColor(tc);
	this->ttype.setPosition(x + 0.1, -0.30);
	this->ttype.setData(L"Type of file:");

	this->type.Set(x + 0.1, -0.36, 0.5, 0.06, bc, tc, tc, font);
	this->type.AddButton(L"Python file");
	this->type.AddButton(L"Text file");
	this->type.AddButton(L"Image");
	this->type.Select(0);
	this->type.visible = true;
}

void NewFileChooser::Render()
{
	if (this->active)
	{
		this->back.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->out.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		this->title.Render();
		this->tname.Render();
		this->name.Render();
		this->ok.Render();
		this->cancel.Render();
		this->ttype.Render();
		this->type.Render();
	}
}

int NewFileChooser::MouseClick(float x, float y)
{
	if (this->type.back.x1 < x && this->type.back.x2 > x && this->type.back.y1 > y && this->type.back.y2 < y)
		this->type.Click(x, y);
	else if (this->name.back.x1 < x && this->name.back.x2 > x && this->name.back.y1 > y && this->name.back.y2 < y)
		this->name.selected = true;
	else if (this->ok.back.x1 < x && this->ok.back.x2 > x && this->ok.back.y1 > y && this->ok.back.y2 < y)
	{
		this->active = false;
		return 1;
	}
	else if (this->cancel.back.x1 < x && this->cancel.back.x2 > x && this->cancel.back.y1 > y && this->cancel.back.y2 < y)
	{
		this->active = false;
		return 2;
	}
	else
	{
		this->name.selected = false;
		this->type.Shrink();
	}
	return 0;
}

void NewFileChooser::KeyPressed(wchar_t key)
{
	if (this->name.selected)
	    this->name.KeyPressed(key);
}
