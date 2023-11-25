#pragma once
#include "GL.h"
#include <vector>
using namespace std;

class Triangle
{
public:
	void Set(float x1, float y1, float x2, float y2, float x3, float y3, Color color);
	void Render();

	float p1, q1, p2, q2, p3, q3;
	Color qolor;
};

