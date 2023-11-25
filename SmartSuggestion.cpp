#include "SmartSuggestion.h"
#include <iostream>
LabelLoadError SmartSuggestion::FormSuggestionLabel(wstring path)
{
	//Load Python keywords
	ifstream infile(path);
	if (infile.fail())
		return PathNotFound;
	std::string line;
	while (std::getline(infile, line))
	{
		int index = line.find_first_of(':');
		if (index == 0)
			return CorruptedFile;
		string w = line.substr(0, index);
		int t = stoi(line.substr(index + 1, line.size() - index));
		this->label.labels.push_back(Suggestion(w, t));
	}

	//Load Python module names

	wchar_t result[MAX_PATH];
	wstring dir = wstring(result, GetModuleFileName(NULL, result, MAX_PATH));
	dir = dir.substr(0, dir.find_last_of('\\'));

	wstring m_path = L"\\Python\\Lib";
	
	this->LoadPythonModules(dir + m_path);

	return NoError;
}

LabelLoadError SmartSuggestion::FormColorLabel(wstring path)
{
	ifstream infile(path);
	if (infile.fail())
		return PathNotFound;
	std::string line;
	while (std::getline(infile, line))
	{
		int index = line.find_first_of(':');
		if (index == -1)	return CorruptedFile;
		float r = (float)stoi(line.substr(0, index)) / 255.0f;

		int index2 = line.find_first_of(':', index + 1);
		if (index2 == -1)	return CorruptedFile;
		float g = (float)stoi(line.substr(index + 1, index2 - index - 1)) / 255.0f;

		float b = (float)stoi(line.substr(index2 + 1, line.size() - index2 - 1)) / 255.0f;

		this->colorlabel.push_back(Color(r, g, b));
	}
	return NoError;
}
#include <iostream>
void SmartSuggestion::Suggest(vector<Text>* t_suggestions, wstring token, SuggestionBoxData* data)
{
	for (auto& s : this->label.labels)
	{
		if (token.size() <= s.word.size())
		{
			if (s.word.substr(0, token.size()) == string(token.begin(), token.end()))
			{
				Text sugg;
				sugg.setFont(data->s_font);
				sugg.setColor(this->colorlabel[s.type]);
				sugg.setData(wstring(s.word.begin(), s.word.end()));
				sugg.setPosition(data->x_position + 0.02, data->y_position - 0.02 - t_suggestions->size() * (data->s_font->font->glcharsize + 0.02));
				t_suggestions->push_back(sugg);
			}
		}
	}
}

void SmartSuggestion::SuggestModules(vector<Text>* t_suggestions, wstring token, SuggestionBoxData* data)
{
	for (int i = 0; i < this->label.py_modules.size(); i++)
	{
		if (token.size() <= this->label.py_modules[i].size())
		{
			if (this->label.py_modules[i].substr(0, token.size()) == token)
			{
				Text sugg;
				sugg.setFont(data->s_font);
				sugg.setColor(this->colorlabel[12]);
				sugg.setData(this->label.py_modules[i]);
				sugg.setPosition(data->x_position + 0.02, data->y_position - 0.02 - t_suggestions->size() * (data->s_font->font->glcharsize + 0.02));
				t_suggestions->push_back(sugg);
			}
		}
	}
}

void SmartSuggestion::LoadPythonModules(wstring directory) //TODO
{
	WIN32_FIND_DATA findfiledata;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile((directory + L"\\*").c_str(), &findfiledata);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findfiledata.cFileName[0] != '.')
			{
				wstring fullpath = directory + L"\\" + findfiledata.cFileName;
				struct stat s;
				stat(string(fullpath.begin(), fullpath.end()).c_str(), &s);
				if (s.st_mode & S_IFDIR)
				{
					if (findfiledata.cFileName[0] != '_')
					{
						//Folder
						if (this->CheckInitPy(fullpath))
						{
							this->label.py_modules.push_back(findfiledata.cFileName);
						}
					}
				}
				else
				{
					//File
				    if (findfiledata.cFileName[0] != '_')
					{
						wstring mod = findfiledata.cFileName;
						mod = mod.substr(0, mod.find_last_of('.'));
						this->label.py_modules.push_back(mod);
					}
				}
			}
		} while (FindNextFile(hFind, &findfiledata) != 0);
		::FindClose(hFind);
	}
}

bool SmartSuggestion::CheckInitPy(wstring lib)
{
	WIN32_FIND_DATA findfiledata;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile((lib + L"\\*").c_str(), &findfiledata);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (wstring(findfiledata.cFileName) == L"__init__.py")
			{
				return true;
			}
					
		} while (FindNextFile(hFind, &findfiledata) != 0);
		::FindClose(hFind);
	}
	return false;
}
