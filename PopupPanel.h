#pragma once
#include "Components.h"
#include "Primitives.h"
#include "Button.h"

class PopupPanel
{
public:
	void Set(float x, float y, float width, float height, Color back);
	void Render();

	Rectq back;
	Rectq out;

	bool visible = false;
};

