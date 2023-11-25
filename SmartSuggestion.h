#pragma once
#include "GL.h"
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Text.h"
using namespace std;

enum LabelLoadError
{
	NoError,
	PathNotFound,
	CorruptedFile
};

enum SuggestionType
{
	Function = 0,
	Definition = 1,
	Operator = 2,
	Object = 3,
	Statement = 4,
	State = 5,
	LibMan = 6,
	DebugWord = 7,
	Exceptions = 8,
	Plain = 9,
	Symbol = 10,
	Characters = 11,
	Import = 12,
	Blank = 13
};

struct SuggestionBoxData
{
	Font* s_font;
	float x_position;
	float y_position;
	float s_width;
	float s_height;
};

struct Suggestion
{
	Suggestion(string vword, int vtype)
	{
		word = vword;
		type = SuggestionType(vtype);
	}
	string word;
	SuggestionType type;
};

struct SuggestionLabel
{
	vector<Suggestion> labels;
	vector<wstring> py_modules;
};

class SmartSuggestion
{
public:
	LabelLoadError FormSuggestionLabel(wstring path);
	LabelLoadError FormColorLabel(wstring path);

	void Suggest(vector<Text>* t_suggestions, wstring token, SuggestionBoxData* data);
	void SuggestModules(vector<Text>* t_suggestions, wstring token, SuggestionBoxData* data);

	SuggestionLabel label;
	vector<Color> colorlabel;
private:
	void LoadPythonModules(wstring directory);
	bool CheckInitPy(wstring lib);
};

