#pragma once
#include "GL.h"
#include "Texture.h"

class Image
{
public:
	float x, y, w, h;
private:
	Texture t;
public:
	void Set(float dx, float dy, float dw, float dh);
	void LoadTexture(string path);
	void Render();
};

