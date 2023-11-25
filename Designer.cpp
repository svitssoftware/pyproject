#include "Designer.h"

void DesignerW::Set(Font* sfont, float swindowx, float swindowy, float glwindowx, float sx, float sy, vector<DefinedColor>* defcolors, vector<wstring>* stypenames)
{
	this->font = sfont;
	this->windowx = swindowx;
	this->windowy = swindowy;
	this->gx = glwindowx;
	this->x1 = sx;
	this->y1 = sy;
	this->definedcolors = defcolors;
	this->typenames = stypenames;
}

void DesignerW::SetEditor(TextEditor* _editor)
{
	this->editor = _editor;
}

void DesignerW::DesignFile()
{
	for (int i = 3; i < this->editor->Lines.size(); i++)
	{
		this->DesignLine(this->editor->Lines[i], i);
	}
	this->designed = true;
}

void DesignerW::Render()
{
	if (this->visible && this->designed)
	{
		glTranslatef(this->transx, this->transy, 0.0f);
		this->MainWindow.Render();
		for (auto& b : this->Buttons)
		{
			b.Render();
		}
		for (auto& l : this->Labels)
		{
			l.Render();
		}
		for (auto& e : this->Entries)
		{
			e.Render();
		}
		for (auto& c : this->CheckButtons)
		{
			c.Render();
		}
		glTranslatef(-this->transx, -this->transy, 0.0f);
	}
}

void DesignerW::MouseDown(float x, float y)
{
	for (int i = 0; i < this->objects.size(); i++)
	{
		if (x > this->objects[i].graphicrepresentation.bounds.x1
			&& x < this->objects[i].graphicrepresentation.bounds.x2
			&& y < this->objects[i].graphicrepresentation.bounds.y1
			&& y > this->objects[i].graphicrepresentation.bounds.y2)
		{
			this->selectedobject = i;
			this->objectmoving = true;
			return;
		}
	}
}

void DesignerW::MouseHover(float x, float y)
{

}

void DesignerW::MouseUp(float x, float y)
{
	if (this->objectmoving)
	{
		this->objectmoving = false;
		this->ModifyObjectPropertyValue(&this->objects[this->selectedobject], 
			&this->objects[this->selectedobject].properties.X_Position, 
			L"\""+to_wstring(FromGlSize(windowx, x - this->MainWindow.back.x1 - this->transx, gx))+L"\"");
		this->ModifyObjectPropertyValue(&this->objects[this->selectedobject],
			&this->objects[this->selectedobject].properties.Y_Position,
			L"\"" + to_wstring(FromGlSize(windowy, this->MainWindow.header.y2 - y, 2)) + L"\"");
		this->MoveObject(&this->objects[this->selectedobject], x, y);
	}
}

void DesignerW::MouseDblClick(float x, float y)
{
}

void DesignerW::DesignLine(TLine& line, int numberof_line)
{
	Designer::DesignerLine designerline;
	wstring ObjectName;
	wstring ObjectType;
	for (int i = 0; i < line.tokens.size(); i++)
	{
		if (line.tokens[i].type != Blank && line.tokens[i].token.size() > 0)
		{
			designerline.tokens.push_back(Designer::DesignerToken(line.tokens[i], i));
		}
	}
	ObjectName = designerline.tokens[0].token.token;
	if (designerline.tokens[1].token.token == L".")
	{
		//Property
		vector<Designer::PropertyValue> PropertyValues;
		for (int i = 4; i < designerline.tokens.size(); i++)
		{
			if (designerline.tokens[i].token.token == L"," || designerline.tokens[i].token.token == L")")
			{
				PropertyValues.push_back(Designer::PropertyValue(
					designerline.tokens[i - 1].token.token.substr(1, designerline.tokens[i - 1].token.token.size() - 2),
					Designer::DesignerEditorTokenLocator(numberof_line, designerline.tokens[i - 1].realtokennum)));
			}
		}
		this->DesignProperties(this->FindObjectByName(ObjectName), designerline.tokens[2].token.token, PropertyValues);
	}
	else if (designerline.tokens[1].token.token == L"=")
	{
		//Object		
		ObjectType = designerline.tokens[2].token.token;
		this->DesignNewObject(ObjectName, ObjectType);
		this->FindObjectByName(ObjectName)->InitializationLineLocator = numberof_line;
		if (designerline.tokens.size() > 5)
		{
			wstring PropertyName;
			vector<Designer::PropertyValue> PropertyValues;
			//Design properties
			for (int i = 4; i < designerline.tokens.size(); i++)
			{
				if (designerline.tokens[i].token.token == L"=")
				{
					PropertyName = designerline.tokens[i - 1].token.token;
				}
				else if (designerline.tokens[i].token.token == L"," || designerline.tokens[i].token.token == L")")
				{
					PropertyValues.push_back(Designer::PropertyValue(
						designerline.tokens[i-1].token.token.substr(1, designerline.tokens[i-1].token.token.size() - 2),
						Designer::DesignerEditorTokenLocator(numberof_line, designerline.tokens[i-1].realtokennum)));
					this->DesignProperties(this->FindObjectByName(ObjectName), PropertyName, PropertyValues);
					PropertyName.clear();
					PropertyValues.clear();
				}
			}

		}
	}

}

void DesignerW::DesignNewObject(wstring ObjectName, wstring ObjectType)
{
	Designer::DesignerObject object;
	object.name = ObjectName;
	
	if (ObjectType == L"Tk")
	{
		object.type = Designer::DesignerWindow;
		this->DesignNewWindow(&object);
	}
	else if (ObjectType == L"Button")
	{
		object.type = Designer::DesignerButton;
		this->designNewButton(&object);
	}
	else if (ObjectType == L"Entry")
	{
		object.type = Designer::DesignerEntry;
		this->designNewEntry(&object);
	}
	else if (ObjectType == L"Label")
	{
		object.type = Designer::DesignerLabel;
		this->designNewLabel(&object);
	}
	else if (ObjectType == L"Checkbutton")
	{
		object.type = Designer::DesignerCheckbutton;
		this->designNewCheckButton(&object);
	}
	else if (ObjectType == L"Menu")
	{
		object.type = Designer::DesignerMenu;
		this->designNewMenu(&object);
	}
	else if (ObjectType == L"Combobox")
	{
		object.type = Designer::DesignerCombobox;
		this->designNewComboBox(&object);
	}
	else if (ObjectType == L"Listbox")
	{
		object.type = Designer::DesignerListbox;
		this->designNewListBox(&object);
	}
	else if (ObjectType == L"Menubutton")
	{
		object.type = Designer::DesignerMenubutton;
		this->designNewMenuButton(&object);
	}
	else if (ObjectType == L"Canvas")
	{
		object.type = Designer::DesignerCanvas;
		this->designNewCanvas(&object);
	}
	else if (ObjectType == L"Scale")
	{
		object.type = Designer::DesignerScale;
		this->designNewScale(&object);
	}
	else if (ObjectType == L"Scrollbar")
	{
		object.type = Designer::DesignerScrollbar;
		this->designNewScrollBar(&object);
	}
	else if (ObjectType == L"Toplevel")
	{
		object.type = Designer::DesignerToplevel;
		this->designNewTopLevel(&object);
	}
	else if (ObjectType == L"Frame")
	{
		object.type = Designer::DesignerFrame;
		this->designNewFrame(&object);
	}
	else if (ObjectType == L"Radiobutton")
	{
		object.type = Designer::DesignerRadiobutton;
		this->designNewRadioButton(&object);
	}
	this->objects.push_back(object);
}

void DesignerW::DesignProperties(Designer::DesignerObject* Object, wstring PropertyName, vector<Designer::PropertyValue> PropertyValues)
{
	if (PropertyName == L"geometry")
	{
		switch (Object->type)
		{
		case Designer::DesignerWindow:
		{
			float xpos, ypos;
			int middle = PropertyValues[0].value.find_first_of('x');
			xpos = stoi(PropertyValues[0].value.substr(0, middle));
			ypos = stoi(PropertyValues[0].value.substr(middle + 1, PropertyValues[0].value.size() - middle));
			xpos = ToGLSize(windowx, xpos)* (gx / 2);
			ypos = ToGLSize(windowy, ypos);
			this->MainWindow.ChangeSize(xpos, ypos);
			break;
		}
		}
	}
	else if (PropertyName == L"title")
	{
		switch (Object->type)
		{
		case Designer::DesignerWindow:
		{
			this->MainWindow.label.data = PropertyValues[0].value;
		}
		}
	}
	else if (PropertyName == L"text")
	{
		switch (Object->type)
		{
		case Designer::DesignerButton:
		{
			this->Buttons[Object->graphicrepresentation.grnumber].text.data = PropertyValues[0].value;
			break;
		}
		case Designer::DesignerLabel:

			this->Labels[Object->graphicrepresentation.grnumber].text.data = PropertyValues[0].value;
			break;
		case Designer::DesignerCheckbutton:
			this->CheckButtons[Object->graphicrepresentation.grnumber].text.data = PropertyValues[0].value;
		}
	}
	else if (PropertyName == L"bg")
	{
		switch (Object->type)
		{
		case Designer::DesignerButton:
		{
			this->Buttons[Object->graphicrepresentation.grnumber].back.qolor = string_to_color(string(PropertyValues[0].value.begin(), PropertyValues[0].value.end()), *this->definedcolors);
		}
		}
	}
	else if (PropertyName == L"fg")
	{
		switch (Object->type)
		{
		case Designer::DesignerButton:
		{
			this->Buttons[Object->graphicrepresentation.grnumber].text.setColor(string_to_color(string(PropertyValues[0].value.begin(), PropertyValues[0].value.end()), *this->definedcolors));
		}
		}
	}
	else if (PropertyName == L"place")
	{
		switch (Object->type)
		{
		case Designer::DesignerButton:
		{
			this->Buttons[Object->graphicrepresentation.grnumber].back.Set(
				this->MainWindow.back.x1 + ToGLSize(this->windowx, stoi(PropertyValues[0].value) * (gx / 2)),
				this->MainWindow.header.y2 - ToGLSize(this->windowy, stoi(PropertyValues[1].value)),
				this->font->font->glcharsize * (this->Buttons[Object->graphicrepresentation.grnumber].text.data.size() + 1),
				ToGLSize(this->windowy, 22),
				this->Buttons[Object->graphicrepresentation.grnumber].back.qolor);
			this->Buttons[Object->graphicrepresentation.grnumber].text.setPosition(
				this->Buttons[Object->graphicrepresentation.grnumber].back.x1 + 0.01,
				this->Buttons[Object->graphicrepresentation.grnumber].back.y1 - 0.01);
			Object->graphicrepresentation.bounds.x1 = this->Buttons[Object->graphicrepresentation.grnumber].back.x1;
			Object->graphicrepresentation.bounds.x2 = this->Buttons[Object->graphicrepresentation.grnumber].back.x2;
			Object->graphicrepresentation.bounds.y1 = this->Buttons[Object->graphicrepresentation.grnumber].back.y1;
			Object->graphicrepresentation.bounds.y2 = this->Buttons[Object->graphicrepresentation.grnumber].back.y2;
			break;
		}
		case Designer::DesignerLabel:
		{			
			this->Labels[Object->graphicrepresentation.grnumber].text.setPosition(
				this->MainWindow.back.x1 + ToGLSize(this->windowx, stoi(PropertyValues[0].value) * (gx / 2)),
				this->MainWindow.header.y2 - ToGLSize(this->windowy, stoi(PropertyValues[1].value)));
			break;
		}
		case Designer::DesignerEntry:
		{
			this->Entries[Object->graphicrepresentation.grnumber].back.Set(
				this->MainWindow.back.x1 + ToGLSize(this->windowx, stoi(PropertyValues[0].value) * (gx / 2)),
				this->MainWindow.header.y2 - ToGLSize(this->windowy, stoi(PropertyValues[1].value)),
				this->font->font->glcharsize * 15,
				ToGLSize(this->windowy, 22),
				this->Entries[Object->graphicrepresentation.grnumber].back.qolor);
			this->Entries[Object->graphicrepresentation.grnumber].out.Set(
				this->MainWindow.back.x1 + ToGLSize(this->windowx, stoi(PropertyValues[0].value) * (gx / 2)),
				this->MainWindow.header.y2 - ToGLSize(this->windowy, stoi(PropertyValues[1].value)),
				this->font->font->glcharsize * 15,
				ToGLSize(this->windowy, 22),
				this->Entries[Object->graphicrepresentation.grnumber].out.qolor);
			this->Entries[Object->graphicrepresentation.grnumber].entry.setPosition(this->Entries[Object->graphicrepresentation.grnumber].back.x1 + 0.01, this->Entries[Object->graphicrepresentation.grnumber].back.y1 - 0.016);
			break;
		}
		case Designer::DesignerCheckbutton:
		{
			this->CheckButtons[Object->graphicrepresentation.grnumber].back.Set(
				this->MainWindow.back.x1 + ToGLSize(this->windowx, (stoi(PropertyValues[0].value) + 3) * (gx / 2)),
				this->MainWindow.header.y2 - ToGLSize(this->windowy, stoi(PropertyValues[1].value) + 5),
				ToGLSize(this->windowx, 10)* (gx / 2),
				ToGLSize(this->windowy, 10),
				this->CheckButtons[Object->graphicrepresentation.grnumber].back.qolor);
			this->CheckButtons[Object->graphicrepresentation.grnumber].out.Set(
				this->MainWindow.back.x1 + ToGLSize(this->windowx, (stoi(PropertyValues[0].value) + 3) * (gx / 2)),
				this->MainWindow.header.y2 - ToGLSize(this->windowy, stoi(PropertyValues[1].value) + 5),
				ToGLSize(this->windowx, 10)* (gx / 2),
				ToGLSize(this->windowy, 10),
				this->CheckButtons[Object->graphicrepresentation.grnumber].out.qolor);
			this->CheckButtons[Object->graphicrepresentation.grnumber].text.setPosition(
				this->CheckButtons[Object->graphicrepresentation.grnumber].back.x2 + 0.01,
				this->CheckButtons[Object->graphicrepresentation.grnumber].back.y1);
			Object->graphicrepresentation.bounds.x1 = this->Buttons[Object->graphicrepresentation.grnumber].back.x1;
			Object->graphicrepresentation.bounds.x2 = this->Buttons[Object->graphicrepresentation.grnumber].back.x2;
			Object->graphicrepresentation.bounds.y1 = this->Buttons[Object->graphicrepresentation.grnumber].back.y1;
			Object->graphicrepresentation.bounds.y2 = this->Buttons[Object->graphicrepresentation.grnumber].back.y2;
			break;
		}
		}
		Object->properties.X_Position.value = PropertyValues[0].value;
		Object->properties.X_Position.tokenlocator = PropertyValues[0].TokenLocator;
		Object->properties.Y_Position.value = PropertyValues[1].value;
		Object->properties.Y_Position.tokenlocator = PropertyValues[1].TokenLocator;
	}
	else if (PropertyName == L"pack")
	{
		switch (Object->type)
		{
		case Designer::DesignerButton:
		{
			this->Buttons[Object->graphicrepresentation.grnumber].back.Set(
				this->MainWindow.back.x1 + (this->MainWindow.back.x2 - this->MainWindow.back.x1 - this->font->font->glcharsize * (this->Buttons[Object->graphicrepresentation.grnumber].text.data.size() + 1))/2,
				this->MainWindow.header.y2,
				this->font->font->glcharsize * (this->Buttons[Object->graphicrepresentation.grnumber].text.data.size() + 1),
				ToGLSize(this->windowy, 22),
				this->Buttons[Object->graphicrepresentation.grnumber].back.qolor);
			this->Buttons[Object->graphicrepresentation.grnumber].text.setPosition(
				this->Buttons[Object->graphicrepresentation.grnumber].back.x1 + 0.01,
				this->Buttons[Object->graphicrepresentation.grnumber].back.y1 - 0.01);
		}
		}
	}
}

void DesignerW::UserModify()
{

}

void DesignerW::UserCreate(Designer::DesignerObjectType type, float x, float y)
{
	switch(type)
	{
	case Designer::DesignerObjectType::DesignerButton:
	{
		wstring name = L"Button";
		name.append(to_wstring(this->Buttons.size()));

		Designer::Button button;
		button.back.Set(x - this->transx, y, (name.size()+1) * this->font->font->glcharsize, ToGLSize(this->windowy, 25), (*this->definedcolors)[146].color);
		button.text.setPosition(x -this->transx + 0.012, y - 0.016);
		button.text.setFont(this->font);
		button.text.setFont(this->font);
		button.text.setColor(Color(0.2, 0.2, 0.2));
		button.text.setData(name);
		this->Buttons.push_back(button);

		Designer::DesignerObject Object;
		Object.type = Designer::DesignerButton;
		Object.properties.X_Position.value = to_wstring(FromGlSize(windowx, x - this->MainWindow.back.x1 - this->transx, gx));
		Object.properties.Y_Position.value = to_wstring(FromGlSize(windowy, this->MainWindow.header.y2 - y, 2));
		Object.properties.BackGroundColor.value = L"gray";
		Object.properties.BackGroundColor.name = L"bg";
		Object.graphicrepresentation.grnumber = this->Buttons.size() - 1;
		Object.name = name;
		this->objects.push_back(Object);
		this->CodifyObject(&this->objects[this->objects.size() - 1]);
		this->CodifyObjectProperty(&this->objects[this->objects.size() - 1], &this->objects[this->objects.size() - 1].properties.BackGroundColor);
		break;
	}
	case Designer::DesignerObjectType::DesignerEntry:
		break;
	case Designer::DesignerObjectType::DesignerLabel:
	{
		wstring name = L"Label";
		name.append(to_wstring(this->Buttons.size()));

		Designer::Label label;
		label.text.setFont(this->font);
		label.text.setColor(Color(0.2, 0.2, 0.2));
		label.text.setData(name);
		label.text.setPosition(x - this->transx + 0.012, y - 0.016);
		this->Labels.push_back(label);

		Designer::DesignerObject Object;
		Object.type = Designer::DesignerLabel;
		Object.properties.X_Position.value = to_wstring(FromGlSize(windowx, x - this->MainWindow.back.x1 - this->transx, gx));
		Object.properties.Y_Position.value = to_wstring(FromGlSize(windowy, this->MainWindow.header.y2 - y, 2));
		Object.graphicrepresentation.grnumber = this->Labels.size() - 1;
		Object.name = name;
		this->objects.push_back(Object);
		this->CodifyObject(&this->objects[this->objects.size() - 1]);
		break;
	}
	case Designer::DesignerObjectType::DesignerCheckbutton:
	{

		break;
	}
	case Designer::DesignerObjectType::DesignerMenu:
		break;
	case Designer::DesignerObjectType::DesignerCombobox:
		break;
	case Designer::DesignerObjectType::DesignerListbox:
		break;
	case Designer::DesignerObjectType::DesignerMenubutton:
		break;
	case Designer::DesignerObjectType::DesignerCanvas:
		break;
	case Designer::DesignerObjectType::DesignerScale:
		break;
	case Designer::DesignerObjectType::DesignerScrollbar:
		break;
	case Designer::DesignerObjectType::DesignerToplevel:
		break;
	case Designer::DesignerObjectType::DesignerFrame:
		break;
	case Designer::DesignerObjectType::DesignerRadiobutton:
		break;
	}
}

void DesignerW::ReDesign()
{
	this->Buttons.clear();
	this->Labels.clear();
	this->Entries.clear();
	this->CheckButtons.clear();
	this->objects.clear();
	this->DesignFile();
}

void DesignerW::CodifyObject(Designer::DesignerObject* Object)
{
	this->editor->Current_Line = this->editor->Lines.size() - 2;
	Object->InitializationLineLocator = this->editor->Current_Line + 1;
	this->editor->Token_Iterator = this->editor->Lines[this->editor->Current_Line].tokens.size() - 1;
	this->editor->Letter_Iterator = this->editor->Lines[this->editor->Current_Line].tokens[this->editor->Token_Iterator].token.size();
	this->editor->Enter();

	//THE CREATE COMPKNENT
	this->editor->Add(L"    ");
	this->editor->Add(Object->name);
	this->editor->Add(L" = ");
	this->editor->Add((*this->typenames)[Object->type]);
	this->editor->Add(L"(text=\"");
	this->editor->Add(Object->name);
	this->editor->Add(L"\")");

	this->editor->Enter();

	//THE PLACE COMPONENT
	this->editor->Add(L"    ");
	this->editor->Add(Object->name);
	//Xpos
	this->editor->Add(L".place(x=\"");
	this->editor->Add(Object->properties.X_Position.value);
	Object->properties.X_Position.tokenlocator = Designer::DesignerEditorTokenLocator(this->editor->Current_Line, this->editor->Token_Iterator);
	//Ypos
	this->editor->Add(L"\", y=\"");
	this->editor->Add(Object->properties.Y_Position.value);
	Object->properties.Y_Position.tokenlocator = Designer::DesignerEditorTokenLocator(this->editor->Current_Line, this->editor->Token_Iterator);
	this->editor->Add(L"\")");
}

void DesignerW::CodifyObjectProperty(Designer::DesignerObject* Object, Designer::DesignerProperty* property)
{
	this->editor->Current_Line = Object->InitializationLineLocator;
	this->editor->Token_Iterator = this->editor->Lines[Object->InitializationLineLocator].tokens.size() - 3;
	this->editor->Letter_Iterator = this->editor->Lines[this->editor->Current_Line].tokens[this->editor->Token_Iterator].token.size();

	this->editor->Add(L", ");
	this->editor->Add(property->name);
	this->editor->Add(L"=\"");
	this->editor->Add(property->value);
	this->editor->Add(L"\"");
}

void DesignerW::ModifyObject(Designer::DesignerObject* Object)
{
}

void DesignerW::ModifyObjectPropertyValue(Designer::DesignerObject* Object, Designer::DesignerProperty* property, wstring value)
{
	this->editor->Lines[property->tokenlocator.line].tokens[property->tokenlocator.token].token = value;
}

void DesignerW::DeleteObject(Designer::DesignerObject* Object)
{
}

Designer::DesignerObject* DesignerW::FindObjectByName(wstring ObjectName)
{
	for (int i = 0; i < this->objects.size(); i++)
	{
		if (this->objects[i].name == ObjectName)
			return &this->objects[i];
	}
	return nullptr;
}

void DesignerW::DesignNewWindow(Designer::DesignerObject* object)
{
	UISettings const ui_settings{};
	auto const accent_color{ ui_settings.GetColorValue(UIColorType::Accent) };
	Color WinColor = Color(accent_color.R / 256.0f, accent_color.G / 256.0f, accent_color.B / 256.0f);
	this->MainWindow.back.Set(this->x1 + 0.01, this->y1 - 0.01 - ToGLSize(windowy, 30), 0.5, 0.7, Color(0.941, 0.941, 0.941));
	this->MainWindow.header.Set(this->x1 + 0.01, this->y1 - 0.01, 0.5, ToGLSize(windowy, 30), WinColor);
	this->MainWindow.label.setFont(this->font);
	this->MainWindow.label.setColor(Color(1.0, 1.0, 1.0));
	this->MainWindow.label.setPosition(this->x1 + 0.03, this->y1 - 0.03);
	this->MainWindow.label.setData(object->name);
}

void DesignerW::designNewButton(Designer::DesignerObject* object)
{
	Designer::Button button;
	button.back.qolor = (*this->definedcolors)[146].color;
	button.text.setFont(this->font);
	button.text.setColor(Color(0.2, 0.2, 0.2));
	object->graphicrepresentation.grnumber = this->Buttons.size();
	this->Buttons.push_back(button);
}

void DesignerW::designNewEntry(Designer::DesignerObject* object)
{
	Designer::Entry entry;
	entry.back.qolor = Color(1.0, 1.0, 1.0);
	entry.out.qolor = Color(0.0, 0.0, 0.0);
	entry.entry.setFont(this->font);
	entry.entry.setColor(Color(0, 0, 0));
	entry.entry.setData(L"");
	this->Entries.push_back(entry);
	object->graphicrepresentation.grnumber = this->Entries.size() - 1;
}

void DesignerW::designNewLabel(Designer::DesignerObject* object)
{
	Designer::Label label;
	label.text.setFont(this->font);
	label.text.setColor(Color(0, 0, 0));
	this->Labels.push_back(label);
	object->graphicrepresentation.grnumber = this->Labels.size() - 1;
}

void DesignerW::designNewCheckButton(Designer::DesignerObject* object)
{
	Designer::CheckButton checkbutton;
	checkbutton.text.setFont(this->font);
	checkbutton.text.setColor(Color(0, 0, 0));
	checkbutton.back.qolor = Color(1, 1, 1);
	checkbutton.out.qolor = Color(0, 0, 0);
	this->CheckButtons.push_back(checkbutton);
	object->graphicrepresentation.grnumber = this->CheckButtons.size() - 1;
}
void DesignerW::designNewMenu(Designer::DesignerObject* object)
{
}

void DesignerW::designNewComboBox(Designer::DesignerObject* object)
{
}

void DesignerW::designNewListBox(Designer::DesignerObject* object)
{
}

void DesignerW::designNewMenuButton(Designer::DesignerObject* object)
{
}

void DesignerW::designNewCanvas(Designer::DesignerObject* object)
{
}

void DesignerW::designNewScale(Designer::DesignerObject* object)
{
}

void DesignerW::designNewScrollBar(Designer::DesignerObject* object)
{
}

void DesignerW::designNewTopLevel(Designer::DesignerObject* object)
{
}

void DesignerW::designNewFrame(Designer::DesignerObject* object)
{
}

void DesignerW::designNewRadioButton(Designer::DesignerObject* object)
{
}

void DesignerW::MoveObject(Designer::DesignerObject* object, float x, float y)
{
	switch (object->type)
	{
	case Designer::DesignerButton:
		this->Buttons[object->graphicrepresentation.grnumber].back.Set(x, y,
			this->Buttons[object->graphicrepresentation.grnumber].back.x2 - this->Buttons[object->graphicrepresentation.grnumber].back.x1,
			this->Buttons[object->graphicrepresentation.grnumber].back.y1 - this->Buttons[object->graphicrepresentation.grnumber].back.y2,
			this->Buttons[object->graphicrepresentation.grnumber].back.qolor);
		this->Buttons[object->graphicrepresentation.grnumber].text.setPosition(x + 0.012, y - 0.016);
		object->graphicrepresentation.bounds.x1 = x;
		object->graphicrepresentation.bounds.y1 = y;
		object->graphicrepresentation.bounds.x2 = this->Buttons[object->graphicrepresentation.grnumber].back.x2;
		object->graphicrepresentation.bounds.y2 = this->Buttons[object->graphicrepresentation.grnumber].back.y2;
		break;
	}
}
