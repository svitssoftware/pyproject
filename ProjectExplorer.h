#pragma once

#include "Text.h"
#include "Button.h"
#include "Project.h"

class ProjectExplorer
{
private:
	Project* project;
	Text title;
	Font* font;
	vector<Button> pfiles;
	Text projname;
	Line l;
public:
	void SetSize(float dx, float dy, float dw, float dh, Color q);
	void SetFont(Font* font);
	void DispProj(Project* proj);
	void Render();

	int PassClick(float x, float y);
	int PassDblClick(float x, float y);

	bool visible = false;
	Rectq back;
};

