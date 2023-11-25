#include "Rectq.h"

void Rectq::Set(float x, float y, float w, float h, Color color, float z)
{
	this->x1 = x;
	this->x2 = x + w;
	this->y1 = y;
	this->y2 = y - h;
	this->qolor = color;
	this->z = z;
}

void Rectq::Render()
{
	glColor3f(this->qolor.r, this->qolor.g, this->qolor.b);
	glBegin(GL_QUADS);
	glVertex3f(this->x1, this->y1, this->z);
	glVertex3f(this->x2, this->y1, this->z);
	glVertex3f(this->x2, this->y2, this->z);
	glVertex3f(this->x1, this->y2, this->z);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
