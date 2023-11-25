#include "PopupPanel.h"

void PopupPanel::Set(float x, float y, float width, float height, Color back)
{
	this->back.Set(x, y, width, height, back);	
	this->out.Set(x, y, width, height, Color(0.9, 0.9, 0.9));
}


void PopupPanel::Render()
{
	if (this->visible)
	{
		this->back.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->out.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
