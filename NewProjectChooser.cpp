#include "NewProjectChooser.h"

void NewProjectChooser::Set(float x, float y, float w, float h, Color bc, Color oc, Color tc, Font* font)
{
	this->back.Set(x, y, w, h, bc);
	this->out.Set(x, y, w, h, oc);

	this->title.setFont(font);
	this->title.setColor(oc);
	this->title.setPosition(x + 0.05, y - 0.1);
	this->title.setData(L"New Project");
	this->title.setScale(2);

	this->tname.setFont(font);
	this->tname.setColor(tc);
	this->tname.setPosition(x + 0.1, y - 0.24);
	this->tname.setData(L"Project name:");

	this->name.Set(x + 0.1, y - 0.3, 0.5, 0.06, bc, tc, tc, font);

	this->tdirectory.setFont(font);
	this->tdirectory.setColor(tc);
	this->tdirectory.setPosition(x + 0.1, y - 0.4);
	this->tdirectory.setData(L"Project directory:");

	this->directory.Set(x + 0.1, y - 0.46, 1, 0.06, bc, tc, tc, font);
	this->bdirectory.Set(x + 1.12, y - 0.465, 0.06, 0.06);
	this->bdirectory.LoadTexture("choosedir.png");

	this->ok.Set(x + w - 0.2, y - h + 0.1, 0.15, 0.05, bc, font, Center);
	this->ok.SetOutline(tc);
	this->ok.SetText(L"Ok", tc);

	this->cancel.Set(x + 2 - 0.4, y - h + 0.1, 0.15, 0.05, bc, font, Center);
	this->cancel.SetOutline(tc);
	this->cancel.SetText(L"Cancel", tc);

	this->ttype.setFont(font);
	this->ttype.setColor(tc);
	this->ttype.setPosition(x + 0.1, -0.30);
	this->ttype.setData(L"Project type:");

	this->type.Set(x + 0.1, -0.36, 0.5, 0.06, bc, tc, tc, font);
	this->type.AddButton(L"Blank");
	this->type.AddButton(L"Designer");
	this->type.Select(0);
	this->type.visible = true;
}

void NewProjectChooser::Render()
{
	if (this->active)
	{
		this->back.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->out.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		this->title.Render();
		this->tname.Render();
		this->name.Render();
		this->ok.Render();
		this->cancel.Render();
		this->ttype.Render();
		this->type.Render();
		this->tdirectory.Render();
		this->directory.Render();
		this->bdirectory.Render();
	}
}

int NewProjectChooser::MouseClick(float x, float y)
{
	if (this->type.back.x1 < x && this->type.back.x2 > x && this->type.back.y1 > y && this->type.back.y2 < y)
		this->type.Click(x, y);
	else if (this->name.back.x1 < x && this->name.back.x2 > x && this->name.back.y1 > y && this->name.back.y2 < y)
		this->name.selected = true;
	else if (this->ok.back.x1 < x && this->ok.back.x2 > x && this->ok.back.y1 > y && this->ok.back.y2 < y)
	{
		this->active = false;
		return 1;
	}
	else if (this->cancel.back.x1 < x && this->cancel.back.x2 > x && this->cancel.back.y1 > y && this->cancel.back.y2 < y)
	{
		this->active = false;
		return 2;
	}
	else if (this->bdirectory.x < x && this->bdirectory.x + this->bdirectory.w > x && this->bdirectory.y > y && this->bdirectory.y - this->bdirectory.h < y)
	{
		IFileOpenDialog* pFileOpen = NULL;
		IShellItem* pShellItem = NULL;
		wchar_t* ppszName = NULL;
		CoInitialize(NULL);
		CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, (void**)(&pFileOpen));
		DWORD dwOptions;
		if (SUCCEEDED(pFileOpen->GetOptions(&dwOptions)))
		{
			pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);
		}
		pFileOpen->SetTitle(L"Select Project Directory");
		if (pFileOpen->Show(0) == S_OK)
		{
			pFileOpen->GetResult(&pShellItem);
			pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &ppszName);
			this->directory.data.data = ppszName;
			CoTaskMemFree(ppszName);
			pShellItem->Release();
			pFileOpen->Release();
			CoUninitialize();
		}
		return 0;
	
	}
	else
	{
		this->name.selected = false;
		this->type.Shrink();
	}
	return 0;
}

void NewProjectChooser::KeyPressed(wchar_t key)
{
	if (this->name.selected)
		this->name.KeyPressed(key);
}
