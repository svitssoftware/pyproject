#include "Polygon.h"

void Poligon::Set(Color color)
{
	this->qolor = color;
}

void Poligon::AddVertice(Vertice v, int ind)
{
	if (ind == -1)
	    this->verts.push_back(v);
	else
	{
		this->verts.insert(verts.begin() + ind, v);
	}
}

void Poligon::Clear()
{
	this->verts.clear();
}

void Poligon::Render()
{
	if (visible)
	{
		if (this->outlined)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(qolor.r, qolor.g, qolor.b);
		glBegin(GL_POLYGON);
		for (auto& value : this->verts)
			glVertex2f(value.x, value.y);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
