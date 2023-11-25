#include "ImageButton.h"

void ImageButton::Set(float x, float y, float width, float height, Color oc, string imagepath)
{
	this->out.Set(x, y, width, height, oc);
	this->image.Set(x + 0.01, y - 0.01, width - 0.02, width - 0.02);
	this->image.LoadTexture(imagepath);
}

void ImageButton::Render()
{
	this->out.Render();

	this->image.Render();
}
