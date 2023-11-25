#include "TextBox.h"

void TextBox::Set(float x, float y, float w, float h, Color bc, Color oc, Color tc, Font* font)
{
	this->back.Set(x, y, w, h, bc);
	this->out.Set(x, y, w, h, oc);
	this->data.setFont(font);
	this->data.setPosition(x + 0.01, y - 0.02);
	this->data.setColor(tc);
	this->Cursor.Set(x + 0.01, y - 0.01, x + 0.01, y - h + 0.02, oc);
}

void TextBox::Add(wchar_t key)
{
	this->data.addData(key);
	this->pos = this->pos + 1;

	this->Cursor.p1 = this->Cursor.p1 + this->data.font->font->chars[(int)key].advance;
	this->Cursor.p2 = this->Cursor.p1;
}

void TextBox::KeyPressed(wchar_t key)
{
	switch (key)
	{
	case 8:
		this->BackSpace();
		break;
	case 13:
		//Enter
		break;
	default:
		this->Add(key);
		break;
	}
}

void TextBox::BackSpace()
{
	if (this->pos > 0)
	{
		this->Cursor.p1 = this->Cursor.p1 - this->data.font->font->chars[(int)this->data.data[this->pos]].advance;
		this->Cursor.p2 = this->Cursor.p1;
		this->data.removeData(this->pos - 1, 1);
		this->pos = this->pos - 1;
	}

}

void TextBox::Render()
{
	this->back.Render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	this->out.Render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	this->data.Render();
	this->Cursor.Render();
}
