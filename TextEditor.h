#pragma once
#include "Components.h"
#include "Primitives.h"
#include "Font.h"
#include "SmartSuggestion.h"
#include <chrono>
#include <iostream>
#include <thread>


struct TToken
{
	wstring token;
	SuggestionType type;
	TToken(wchar_t _token, SuggestionType _type)
	{
		token = _token;
		type = _type;
	}
	TToken(wstring _token = L"", SuggestionType _type = Plain)
	{
		token = _token;
		type = _type;
	}
};

struct TLine
{
	vector<TToken> tokens;
	int size = 0;
};

struct SuggestionBox
	{
		SuggestionBoxData data;
		bool visible;
		Rectq back;
		Rectq out;
		vector<Text> suggestions;
		void Render();
	};

struct Locator
	{
		int line;
		int token;
		int letter;
		bool operator==(Locator& const second)
		{
			if (line == second.line && token == second.token && letter == second.letter)
				return true;
			return false;
		}
	};

struct Selection
	{
		Locator start;
		Locator end;
		vector<Rectq> markers;
	};

struct Error
{
	int nline;
	int ntoken;
};

class TextEditor
{
#pragma region Graphics
public:
	TextEditor();

	Rectq         w_border;
	Rectq         border;
	Line          Cursor;
	vector<TLine> Lines;
	int           Current_Line = -1;
	int           Current_Line_Position = 0;
	float         cx;
	float         yborder = 0.0f;
	Font*         font;
	bool          visible = true;
	int           startline = 0;
	int           maxlines = 0;

	void Set(float x, float y, float width, float height, Color color, Font* vfont);
	void SetDesignerBelongine(int d);
	void Resize(float x, float y);
	void Render();
	void RenderLines();

	void Add(wstring keys, bool loading = true)
	{
		for (wchar_t key : keys)
			Add(key, loading);
	}
	void Add(wchar_t key, bool loading);
	void Reset          ();                                  //DONE
	void Enter          ();                                  //DONE
	void Backspace      ();                              //DONE
	void Delete         ();                                 //DONE
	void MoveLeft       ();                               //DONE
	void MoveRight      ();
	void MoveUp         ();
	void MoveDown       ();
	void MouseWheel     (short delta);                  //DONE
	void ChangePosition (Locator& locator);
	void MouseDown      (float x, float y);
	void MouseUp        (float x, float y);
	void MouseMove      (float x, float y);
	void CursorOnEnd    ();                            
	void keyDown        (int key);                         //DONE
	void keyClick       (wchar_t key);                    //DONE
	void MouseHover     (float x, float y);
	void Copy           ();
	void Paste          (wstring data);
	void DblClick       (float x, float y);

	void Suggest();
	void HideSuggestion();
	void PushNewLine(int index = -1);
private:
#pragma endregion
#pragma region Code
public:
	SmartSuggestion* smart;
	SuggestionBox suggestionbox;

	int  Token_Iterator = 0;
	int  Letter_Iterator = 0;

	bool t_token = false;
	void Tokenize();
	int  CalcLineLenght(TLine& line);

	int designernum = -1; //-1..It belongs to no designer, otherwise specified 0 or more
public:
	vector<wstring> variables;
	vector<wstring> imports;
	bool importing = false;

	//Temporary information
	void LineChanged(int c);
	vector<int> changedlines;

	//Information
	bool in_brackets = false;

	Selection selection;
	bool selecting = false;
	bool showselection = false;
#pragma endregion
};