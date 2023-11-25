#include "Image.h"

void Image::Set(float dx, float dy, float dw, float dh)
{
	this->x = dx;
	this->y = dy;
	this->w = dw;
	this->h = dh;
}

void Image::LoadTexture(string path)
{
	this->t.LoadFromFile(path);
}

void Image::Render()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, this->t.texName);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(this->x, this->y, 0.1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(this->x + w, this->y, 0.1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(this->x + w, this->y - h, 0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(this->x, this->y - h, 0.1);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, 0);
}
