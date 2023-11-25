#pragma once

#include <windows.h> 
#include <windowsx.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment (lib, "opengl32.lib")  
#pragma comment (lib, "glu32.lib")

#include <string>
using namespace std;

static int WIDTH;
static int HEIGHT;
static float glWIDTH;

struct Color
{
	Color() {}
	Color(float rr, float gg, float bb)
	{
		r = rr;
		g = gg;
		b = bb;
	}
	float r = 0.0f, g = 0.0f, b = 0.0F;
};

static bool EqualColors(Color c1, Color c2)
{
	if (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b)
		return true;
	return false;
}


static float ToGLSize(float w, float x)
{
	float wh = w / 2;
	return x / wh;
}

static int FromGlSize(float w, float x, float gw)
{
	float wh = w / gw;
	return wh * x;
}


static struct DefinedColor
{
	DefinedColor(string nname, Color ccolor)
	{
		name = nname;
		color = ccolor;
	}
	string name;
	Color color;
};