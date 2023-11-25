#pragma once
#include "Image.h"
#include "Rectq.h"

class ImageButton
{
public:
	Image image;
	Rectq out;

	void Set(float x, float y, float width, float height, Color oc, string imagepath);
	void Render();
};

