#include "Button.h"

void Button::Set(float x, float y, float width, float height, Color c, Font* vfont, Anchor anchor, float z)
{
	this->back.Set(x, y, width, height, c, z);
	this->text.setPosition(x + 0.01f, y - 0.01f);
	this->text.setFont(vfont);
	this->an = anchor;
	this->out.Set(x, y, width, height, c);
}

void Button::SetOutline(Color oc)
{
	this->outlined = true;
	this->out.qolor = oc;
}

void Button::SetText(wstring text, Color tc, bool autosize)
{
	this->text.setData(text);
	this->text.setColor(tc);

	switch (this->an)
	{
	case Center:
	{
		float xsize = 0.0f;
		for (wchar_t& key : text)
		{
			xsize = xsize + this->text.font->font->chars[(int)key].advance;
		}
		float xpos = (this->back.x2 - this->back.x1 - 0.02f - xsize) / 2.0f;
		this->text.setPosition(this->text.x + xpos, this->text.y);
		break;
	}
	case Left:
	{
		if (autosize)
		{
			float xsize = 0;
			for (wchar_t& key : text)
			{
				xsize = xsize + this->text.font->font->chars[(int)key].advance;
			}
			this->back.x2 = this->back.x1 + xsize + 0.02f;
		}
	}
	}
}

void Button::SizeToText()
{
	/*Char* ch = nullptr;
	float xpos = 0;
	for (int index = 0; index < this->text.data.size(); index++)
	{
		ch = &this->text.font->font->chars[(int)this->text.data[index]];
		xpos = xpos + ch->advance;
	}*/

	if (this->text.orientation == 0)
	{

	}
	else if (this->text.orientation == 1)
	{
		this->back.Set(this->back.x1, this->back.y1, this->back.y1 - this->back.y2, this->back.x2 - this->back.x1, this->back.qolor);
	}
}

void Button::Render()
{
	if (this->visible)
	{
		this->back.Render();
		if (this->outlined)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			this->out.Render();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		this->text.Render();
	}
}
