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
#include <winrt/Windows.UI.ViewManagement.h>
using namespace winrt;
using namespace Windows::UI::ViewManagement;

#include <fstream>
#include <string>
using namespace std;

namespace Designer
{
#pragma region DesignerObject
	enum DesignerObjectType
	{
		DesignerButton = 0,
		DesignerEntry = 1,
		DesignerLabel = 2,
		DesignerCheckbutton = 3,
		DesignerMenu = 4,
		DesignerCombobox = 5,
		DesignerListbox = 6,
		DesignerMenubutton = 7,
		DesignerCanvas = 8,
		DesignerScale = 9,
		DesignerScrollbar = 10,
		DesignerToplevel = 11,
		DesignerFrame = 12,
		DesignerRadiobutton = 13,
		DesignerWindow = 14
	};

	struct DesignerToken
	{
		TToken token;
		int realtokennum;
		DesignerToken(TToken _token, int _realtokennum)
		{
			token = _token;
			realtokennum = _realtokennum;
		}
	};
	struct DesignerLine
	{
		vector<DesignerToken> tokens;
	
	};
	struct DesignerEditorTokenLocator
	{
		int line;
		int token;
		DesignerEditorTokenLocator(int _line = 0, int _token = 0)
		{
			line = _line;
			token = _token;
		}
	};

	struct DesignerProperty
	{
		wstring name;
		wstring value;
		DesignerEditorTokenLocator tokenlocator;
	};

	struct DesignerProperties
	{
		DesignerProperty X_Position;
		DesignerProperty Y_Position;
		DesignerProperty Text;
		DesignerProperty BackGroundColor;
	};

	struct DesignerBounds
	{
		float x1=0, x2=0, y1=0, y2=0;
	};

	struct DesignerGraphicRepresentation
	{
		int grnumber;
		DesignerBounds bounds;
	};

	struct DesignerObject
	{
		wstring name;
		DesignerObjectType type;
		DesignerProperties properties;
		DesignerGraphicRepresentation graphicrepresentation;
		int InitializationLineLocator = -1;
	};
#pragma endregion
#pragma region GraphicRepresentations
	struct Window
	{
		Rectq back;
		Rectq header;
		Text label;

		void ChangeSize(float width, float height)
		{
			back.x2 = back.x1 + width;
			back.y2 = back.y1 - height;
			header.x2 = back.x2;
		}

		void Render()
		{
			back.Render();
			header.Render();
			label.Render();
		};
	};

	struct Button
	{
		Rectq back;
		Text text;

		void Render()
		{
			back.Render();
			text.Render();
		};
	};

	struct Entry
	{
		Rectq back;
		Rectq out;
		Text entry;
		Line Cursor;

		void Render()
		{
			back.Render();
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			out.Render();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			entry.Render();
		};
	};

	struct Label
	{
		Text text;

		void Render()
		{
			text.Render();
		};
	};

	struct CheckButton
	{
		Text text;
		Rectq back;
		Rectq out;

		void Render()
		{
			back.Render();
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			out.Render();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			text.Render();
		}
	};

	struct Menu
	{

	};

	struct ComboBox
	{

	};

	struct ListBox
	{

	};

	struct MenuButton
	{

	};

	struct Canvas
	{

	};

	struct Scale
	{

	};

	struct ScrollBar
	{

	};

	struct TopLevel
	{

	};

	struct Frame
	{

	};

	struct RadioButton
	{

	};
#pragma endregion

	struct PropertyValue
	{
		wstring value;
		DesignerEditorTokenLocator TokenLocator;
		PropertyValue(wstring _value, DesignerEditorTokenLocator _TokenLocator)
		{
			value = _value;
			TokenLocator = _TokenLocator;
		}
	};
}

class DesignerW
{
public:
	void Set(Font* sfont, float swindowx, float swindowy, float glwindowx, float sx, float sy, vector<DefinedColor>* defcolors, vector<wstring>* stypenames);
	void SetEditor(TextEditor* _editor);
	void DesignFile();
	void Render();

	Font* font;
    float gx;
	float windowx, windowy;
	float transx = 0.0f, transy = 0.0f;
	bool designed = false;

	bool visible = false;

	void MouseDown(float x, float y);
	void MouseHover(float x, float y);
	void MouseUp(float x, float y);
	void MouseDblClick(float x, float y);

	Designer::Window              MainWindow;
	vector<Designer::Button>      Buttons;
	vector<Designer::Entry>       Entries;
	vector<Designer::Label>       Labels;
	vector<Designer::CheckButton> CheckButtons;
	vector<Designer::Menu>        Menus;
	vector<Designer::ComboBox>    ComboBoxes;
	vector<Designer::ListBox>     ListBoxes;
	vector<Designer::MenuButton>  MenuButtons;
	vector<Designer::Canvas>      Canvases;
	vector<Designer::Scale>       Scales;
	vector<Designer::ScrollBar>   ScrollBars;
	vector<Designer::TopLevel>    TopLevels;
	vector<Designer::Frame>       Frames;
	vector<Designer::RadioButton> Radiobuttons;

	void UserCreate(Designer::DesignerObjectType type, float x, float y);
	float x1, x2, y1, y2;

	void ReDesign();
	TextEditor* editor;
private:
	void DesignLine(TLine& line, int numberof_line);
	void DesignNewObject(wstring ObjectName, wstring ObjectType);
	void DesignProperties(Designer::DesignerObject* Object, wstring PropertyName, vector<Designer::PropertyValue> PropertyValue);
	void UserModify();
	void CodifyObject(Designer::DesignerObject* Object);
	void CodifyObjectProperty(Designer::DesignerObject* Object, Designer::DesignerProperty* property);
	void ModifyObject(Designer::DesignerObject* Object);
	void ModifyObjectPropertyValue(Designer::DesignerObject* Object, Designer::DesignerProperty* property, wstring value);
	void DeleteObject(Designer::DesignerObject* Object);
	Designer::DesignerObject* FindObjectByName(wstring ObjectName);

	vector<Designer::DesignerObject> objects;

	void DesignNewWindow     (Designer::DesignerObject* object);
	void designNewButton     (Designer::DesignerObject* object);
	void designNewEntry      (Designer::DesignerObject* object);
	void designNewLabel      (Designer::DesignerObject* object);
	void designNewCheckButton(Designer::DesignerObject* object);
	void designNewMenu       (Designer::DesignerObject* object);
	void designNewComboBox   (Designer::DesignerObject* object);
	void designNewListBox    (Designer::DesignerObject* object);
	void designNewMenuButton (Designer::DesignerObject* object);
	void designNewCanvas     (Designer::DesignerObject* object);
	void designNewScale      (Designer::DesignerObject* object);
	void designNewScrollBar  (Designer::DesignerObject* object);
	void designNewTopLevel   (Designer::DesignerObject* object);
	void designNewFrame      (Designer::DesignerObject* object);
	void designNewRadioButton(Designer::DesignerObject* object);

	vector<DefinedColor>* definedcolors;
	vector<wstring>* typenames;

	int selectedobject = -1;
	bool objectmoving = false;

	void MoveObject(Designer::DesignerObject* object, float x, float y);
};

