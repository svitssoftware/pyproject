#pragma once
#include "GL.h"

class Line
{
public:
	void Set(float x1, float y1, float x2, float y2, Color color);
	void LineStipple(int factor, short pattern);
	void Render();

	float p1, q1, p2, q2;
	Color qolor;
	int qactor = 1;
	short qattern = 0xFFFF;
};

