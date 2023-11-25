#include "Triangle.h"

void Triangle::Set(float x1, float y1, float x2, float y2, float x3, float y3, Color color)
{
	this->p1 = x1;
	this->p2 = x2;
	this->p3 = x3;
	this->q1 = y1;
	this->q2 = y2;
	this->q3 = y3;

	this->qolor = color;
}

void Triangle::Render()
{
	glColor3f(this->qolor.r, this->qolor.g, this->qolor.b);
	glBegin(GL_TRIANGLES);
	glVertex2f(this->p1, this->q1);
	glVertex2f(this->p2, this->q2);
	glVertex2f(this->p3, this->q3);
	glEnd();
}
