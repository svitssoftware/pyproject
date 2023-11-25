#pragma once

#include "GL.h"
#include "stb_image.hpp"
#include <string>

using namespace std;

class Texture
{
public:
	Texture();
	void LoadFromFile(string path);
	float x, y;
	unsigned int texName;
};
