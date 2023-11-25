#pragma once
#include <string>
#include <vector>
#include <fstream>
using namespace std;

enum ProjectType
{
	BlankProject,
	DesignerProject
};

struct ProjectSettings
{
	wstring projname;
	string projpath;
	ProjectType type;
};

class Project
{
public:
	ProjectSettings settings;
	vector<string> projfiles;
	vector<int> projdesignerfiles;

	bool LoadProject(string path);
	void CreateProject(string directory, string name, ProjectType type);
};

