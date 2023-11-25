#include "Line.h"

void Line::Set(float x1, float y1, float x2, float y2, Color color)
{
	this->p1 = x1;
	this->p2 = x2;
	this->q1 = y1;
	this->q2 = y2;
	this->qolor = color;
}

void Line::LineStipple(int factor, short pattern)
{
	this->qactor = factor;
	this->qattern = pattern;
}

void Line::Render()
{
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(this->qactor, this->qattern);
	glColor3f(this->qolor.r, this->qolor.g, this->qolor.b);
	glBegin(GL_LINES);
	glVertex2f(this->p1, this->q1);
	glVertex2f(this->p2, this->q2);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}
