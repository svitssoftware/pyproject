#pragma once
#include "GL.h"

class Rectq
{
public:
	void Set(float x, float y, float w, float h, Color color, float z = 0);
	void Render();

	float x1, y1, x2, y2, z;
	Color qolor;
};

