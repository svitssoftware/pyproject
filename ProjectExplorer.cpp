#include "ProjectExplorer.h"

void ProjectExplorer::SetSize(float dx, float dy, float dw, float dh, Color q)
{
	this->back.Set(dx, dy, dw, dh, q);
	this->title.setFont(this->font);
	this->title.setPosition(dx + 0.01, dy - 0.015);
	this->title.setData(L"Project Explorer");
	this->title.setColor(Color(0.9f, 0.9f, 0.9f));
	this->l.Set(dx, dy- 0.04, dx + dw, dy - 0.04, Color(0.44313725490, 0.37647058824, 0.90980392157));
	this->projname.setFont(this->font);
	this->projname.setPosition(dx + 0.027, dy - 0.08);
	this->projname.setColor(Color(0.29803921569, 0.60000000000, 0.85882352941));
}

void ProjectExplorer::SetFont(Font* font)
{
	this->font = font;
}

void ProjectExplorer::DispProj(Project* proj)
{
	this->project = proj;
	this->pfiles.clear();
	this->projname.addData(proj->settings.projname);
	for (int i = 0; i < this->project->projfiles.size(); i++)
	{
		Button b;
		b.Set(this->back.x1 + 0.04, this->back.y1 - (i + 1) * (0.04) - 0.08, this->back.x2 - this->back.x1 - 0.2, 0.04, this->back.qolor, this->font, Anchor::Left);
		int off = proj->projfiles[i].find_last_of('\\');
		string path = proj->projfiles[i].substr(off + 1, proj->projfiles[i].size() - off - 1);
		b.SetText(wstring(path.begin(), path.end()), Color(0.9f, 0.9f, 0.9f));
		this->pfiles.push_back(b);
	}
}

void ProjectExplorer::Render()
{
	if (this->visible)
	{
		this->back.Render();
		this->title.Render();
		for (auto& each : this->pfiles)
			each.Render();
		this->projname.Render();
		this->l.Render();
	}
}

int ProjectExplorer::PassClick(float x, float y)
{
	for (int i = 0; i < this->pfiles.size(); i++)
	{
		if (x > this->pfiles[i].back.x1 && x < this->pfiles[i].back.x2 && y < this->pfiles[i].back.y1 && y > this->pfiles[i].back.y2)
		{
			return i;
		}
	}
	return -1;
}

int ProjectExplorer::PassDblClick(float x, float y)
{
	return -1;
}
