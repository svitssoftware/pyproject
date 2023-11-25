#include "Project.h"

bool Project::LoadProject(string path)
{
    this->projfiles.clear();
    this->settings.projname.clear();
    this->settings.projpath.clear();
    this->projdesignerfiles.clear();

	std::ifstream ifs(path);

    std::string line;
    int noline = 0;
    bool df = false;
    bool ds = false;
    bool ddf = false;
    while (std::getline(ifs, line))
    {
        switch (noline)
        {
        case 0:
            this->settings.projname = wstring(line.begin(), line.end());
            break;
        case 1:
            switch (line[0])
            {
            case '0':
                this->settings.type = BlankProject;
                break;
            case '1':
                this->settings.type = DesignerProject;
                break;
            }
            break;
        case 2:
            this->settings.projpath = line;
            break;
        default:
            if (df == false && ds == false)
                df = true;

            if (df && line != "-DF" && line != "-DDF")
            {
                if (!ddf)
                {
                    this->projfiles.push_back(line);
                }
                else
                {
                    this->projfiles.push_back(line);
                    this->projdesignerfiles.push_back(this->projfiles.size() - 1);
                    ddf = false;
                }
            }
            else if (df && line == "-DDF")
            {
                ddf = true;
            }
            else if (ds)
            {

            }
            else
            {
                df = false;
                ds = true;
            }
            break;
        }
        noline = noline + 1;
    }
	return true;
}

void Project::CreateProject(string directory, string name, ProjectType type)
{
    switch (type)
    {
    case BlankProject:
    {
        ofstream ProjectFile(directory + name + ".pyproj");

        string content;
        content.append(name);
        content.append("\n");
        content.append("0");
        content.append("\n");
        content.append(directory);
        content.append("\n");
        content.append(name + ".py");

        ProjectFile << content;
        ProjectFile.close();


        ofstream PythonFile(directory + name + ".py");
           
        content.clear();
        content.append("The beginning of ");
        content.append(name);
        content.append("...");

        PythonFile << content;
        PythonFile.close();
        break;
    }
    }
}
