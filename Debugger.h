#pragma once
#include <Python.h>
#include "TextEditor.h"
#include <fstream>
#include <string>
using namespace std;

class Debugger
{
public:
	Debugger();
	~Debugger();

	void Start();
	void End();
	void Stop();

	void SaveLines(wstring path, vector<TLine>* Lines);
	void StartProgram(wstring path, string workdir);
};

