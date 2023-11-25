#include "Text.h"


Text::Text()
{
}

Text::~Text()
{
}

void Text::setColor(Color vc)
{
	this->c = vc;
}

void Text::setPosition(float vx, float vy)
{
	this->x = vx;
	this->y = vy;
}

void Text::setFont(Font* vfont)
{
	this->font = vfont;
	this->fa = this->font->font;
}

void Text::setScale(float vscale)
{
	if (vscale > 1)
	{
		this->fa = this->font->sfont;
		int ch = vscale * 15;
		this->scale = ch / 30;
	}
	else
	{
		this->fa = this->font->font;
	}
}

void Text::setData(wstring vdata)
{
	this->data = vdata;
}

void Text::addData(wchar_t vcdata)
{
	this->data.push_back(vcdata);
}

void Text::removeData(int start, int count)
{
	this->data.erase(start, count);
}

#include <iostream>
void Text::Render()
{
	if (this->orientation == 0)
	{
		float xpos = this->x, rowpos = this->y - this->fa->glcharsize;
		glColor3f(this->c.r, this->c.g, this->c.b);
		for (int n = 0; n < this->data.size(); n++)
		{			
			wchar_t wch = this->data[n];
			switch (wch)
			{
			case L' ':
			{
				xpos = xpos + this->fa->glcharsize / 4 * this->font->factor;
				break;
			}
			default:
			{
				Char* ch = &this->fa->chars[(int)wch];

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				glBindTexture(GL_TEXTURE_2D, ch->texName);

				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos + ch->xbearing, rowpos + ch->ybearing);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + ch->xbearing + this->scale * ch->sizex, rowpos + ch->ybearing);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + ch->xbearing + this->scale * ch->sizex, rowpos + ch->ybearing - this->scale * ch->sizey);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos + ch->xbearing, rowpos + ch->ybearing - this->scale * ch->sizey);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, 0);

				xpos = xpos + this->scale * ch->advance;

				break;
			}
			}
		}
	}
	else if (this->orientation == 1)
	{
		float xpos = this->x, rowpos = this->y;

		for (int n = 0; n < this->data.size(); n++)
		{
			wchar_t wch = this->data[n];

			switch (wch)
			{
			case L' ':
			{
				rowpos = rowpos - this->font->font->glcharsize / 4 * this->font->factor;
				break;
			}
			default:
			{
				Char* ch = &this->font->font->chars[(int)wch];

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				glBindTexture(GL_TEXTURE_2D, ch->texName);

				glColor3f(this->c.r, this->c.g, this->c.b);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos - ch->sizey + ch->ybearing, rowpos - ch->xbearing);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos - ch->sizey + ch->ybearing, rowpos - ch->xbearing - ch->sizex);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + ch->ybearing, rowpos - ch->xbearing - ch->sizex);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos + ch->ybearing, rowpos - ch->xbearing);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, 0);

				rowpos = rowpos - ch->advance;

				break;
			}
			}
		}
	}
}
