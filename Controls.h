#pragma once
#include <vector>
using namespace std;

namespace Controls
{
	enum ControlType
	{
		Main,
		Button,
		TextEditor,
		Panel,
		Window,
		Designer
	};

    struct Control
    {
		int index = -1;
		int ID = -1;
		int command = -1;
		bool extr = false;
    	ControlType type;
    };
};