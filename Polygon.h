#pragma once
#include "GL.h"
#include <vector>
using namespace std;

struct Vertice
{
	float x = 0, y = 0;
	Vertice()
	{

	}
	Vertice(float dx, float dy)
	{
		x = dx;
		y = dy;
	}
};

class Poligon
{
public:
	void Set(Color color);
	void AddVertice(Vertice v, int ind = -1);
	void Clear();
	void Render();

	Color qolor;
	vector<Vertice> verts;
	bool outlined = false;
	bool visible = false;
};

