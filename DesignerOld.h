#pragma once
#include "Primitives.h"
#include "Image.h"
#include "Button.h"
#include "PopupPanel.h"
#include "TextBox.h"
#include "ToolBox.h"
#include "TextEditor.h"
#include "DPropertyObject.h"
#include "DefinedColors.h"

#include <fstream>
#include <string>
using namespace std;

namespace Designer
{
#pragma region structs
	enum DesignerObjectType
	{
		d_Button      = 0,
		d_Entry       = 1,
		d_Label       = 2,
		d_Checkbutton = 3, 
		d_Menu        = 4,
		d_Combobox    = 5,
		d_Listbox     = 6,
		d_Menubutton  = 7,
		d_Canvas      = 8,
		d_Scale       = 9,
		d_Scrollbar   = 10,
		d_Toplevel    = 11,
		d_Frame       = 12,
		d_RadioButton = 13,
		d_Window      = 14
	};

	class dWindow
	{
	public:
		Rectq back;
		Rectq titleback;
		Text title;
	};

	class dButton
	{
	public:
		Button button;
	};

	struct DesignerObject
	{
		wstring name;
		int index;
		int lineint;

		DesignerObjectType type;
		DProperties properties;
	};

	struct dParserLine
	{
		vector<wstring> tokens;
	};

	struct DParserTree
	{
		vector<dParserLine> lines;
	};
	
#pragma endregion
#pragma region class
	class Designer
	{
	public:
		vector<DesignerObject> objects;
		
		dWindow dwin;
		vector<dButton> dbuttons;

		bool visible = true;
		float ww, wh, gww;
		float zx, zy;
		void Set(float dx, float dy, float w, float h, Font* font, Color color);
		void ConEditor(TextEditor* ed);
		void SetSPL(vector<vector<int>>* ospl, vector<wstring>* nspl);
		void SetWinSize(float w, float h, float gw);
		void Render();

		void dMouseDown(POINT md);

		void ShapeAfterFile(wifstream& input);
		void ProcessDefineSetObjectDefinition(vector<wstring>& tokens, DesignerObject* object);

		void DesignerLineChanged(vector<int> clines);
		void ProcessLine(TLine& line);

		vector<DefinedColor>* definedcolors;
	private:
		TextEditor* con_editor;
		vector<vector<int>>* obj_spec_prop_lists; //For object type specific property lists
		vector<wstring>*     not_spec_prop_list;  //For all objects complete property list
	};
#pragma endregion
#pragma region commentlists
//OBJ_SPEC_PROP_LIST
//     0 - Button
// 	   1 - Entry
// 	   2 - Label
// 	   3 - Checkbutton
// 	   4 - Menu
// 	   5 - Combobox
// 	   6 - Listbox
// 	   7 - Menubutton
// 	   8 - Canvas
// 	   9 - Scale
// 	   10 - Scrollbar
// 	   11 - Toplevel
// 	   12 - Frame
// 	   13 - Radiobutton
//     14 - Window
#pragma endregion
};

