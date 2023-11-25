#pragma once

#include <fstream>
#include <stdio.h> 
#include <conio.h> 
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>
#include <cstdlib>
#include "Primitives.h"
#include "Text.h"
#include "TextEditor.h"
#include "Debugger.h"
#include "Image.h"
#include "Designer.h"
#include "Button.h"
#include "PopupPanel.h"
#include "Controls.h"
#include "Project.h"
#include "ProjectExplorer.h"
#include "NewFileChooser.h"
#include "NewProjectChooser.h"
#include "ToolBox.h"

enum TabDataType
{
    ProjectFile,
	OpenedFile,
	pDesignerFile,
	fDesignerFile,
	UnsavedNewFile
};

struct Tab
{
	Button button;
	int index;
	TabDataType holdtype;
	wstring path_to_file;
	bool linked_to_file;
};

class Editor
{
public: 
	Project project;

	Editor(HWND* hwnd);
	void Load(int w, int h, float glw);
	void LoadProject(string path);
	void LoadSPL();
	void LoadLayout();
	void Render();

	void keyClick(wchar_t key);
	void keyDown(int key);
	void mouseDown(int x, int y);
	void mouseUp(int x, int y);
	void mouseWheel(short delta);
	void mouseMove(int x, int y);
	void mouseDblClick(int x, int y);

	POINT immediateCursorPos;
	void processcoord(float& x);
private:
	HWND* target;
	bool qrefreshing = true;
	int qtimer = 0;

	Debugger* debugger;
	Font font;
	SmartSuggestion smart;

	void resize(float height, float width);
	bool enterFullscreen(HWND hwnd);
	bool exitFullscreen(HWND hwnd);

	vector<vector<int>> ospl;
	vector<wstring>     nspl;
	//Layout
private:
	int count_temp = -1;

	vector<Button> l_buttons;
	Poligon h_button;
	void AddButton(float x, float y, float width, float height, Color c, Font* vfont, wstring text, Color tc, Anchor an, float z, int command = 0, bool visible = true, int parent = -1);

	vector<Text*> l_texts;
	vector<TextEditor*> l_editors;
	vector<PopupPanel*> l_panels;
	void AddPanel(float x, float y, float width, float height, Color c, bool visible = true, int parent = -1);

	vector<Tab*> tabs;
	int selectedtab = -1;

	Line l_upb;
	Line m_upb;
	Line r_upb;

	ProjectExplorer exp;
	void PESwitch();

	ToolBox toolbox;
	void ToolBoxSwitch();

	vector<DesignerW> l_designers;

	Image im;
private:
	vector<Controls::Control> controls;
	Controls::Control* selected;

	Controls::Control* SearchControls(float x, float y);

	void ButtonClick(Controls::Control* id, int count);
	void HideChilds(Controls::Control* id, int count);
	void HideAllChilds();
	void ChangeVisible(Controls::Control* id);
private:
	void NewFile();
	void CreateNewFile(wstring name);
	void CreateNewProject(wstring name, int type);
	void NewProject();
	void OpenFile();
	void OpenFile(wstring path, int i, bool inproject);
	void OpenDesigner(TextEditor* editor);
	void OpenProject();
	void OpenProject(wstring path);
	void SaveFile();
	void SaveAll();

	void Run();

private:
	NewFileChooser New_File_Chooser;
	NewProjectChooser New_Project_Chooser;

	int Procedure = 0;
private:
	vector<DefinedColor> definedcolors;
private:
	void ProcessLineChanges();
	bool ifDesignerTextEditor(int editor);
	vector<wstring> typenames;
private:
	int drag = -1;
	Designer::Button DragButton;
	Designer::Entry DragEntry;
	Designer::Label DragLabel;
};

void TokenizeLine(vector<TLine>* lines, DesignerW* designer);
SuggestionType TokenizeToken(wstring token, SmartSuggestion* smart);

