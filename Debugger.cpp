#include "Debugger.h"

Debugger::Debugger()
{
	Py_Initialize();
}

Debugger::~Debugger()
{
	Py_Finalize();
}

void Debugger::Start()
{
	Py_Initialize();
}

void Debugger::End()
{
	Py_Finalize();
}

void Debugger::Stop()
{
	
}

void Debugger::SaveLines(wstring path, vector<TLine>* Lines)
{
	
}

void Debugger::StartProgram(wstring path, string workdir)
{
	string filename(path.begin(), path.end());
	FILE* fp;
	fopen_s(&fp, filename.c_str(), "r");
	
	string path_append = "sys.path.append(\"" + workdir + "\")";
	PyRun_SimpleString("import sys");
	PyRun_SimpleString(path_append.c_str());

	PyRun_AnyFile(fp, filename.c_str());
	fclose(fp);
}