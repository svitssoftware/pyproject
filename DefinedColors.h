#pragma once
#include "GL.h";
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

static Color string_to_color(string color, vector<DefinedColor>& definedcolors)
{
	bool found = false;
	int s = 0;
	int e = definedcolors.size();
	int mid;
	while (!found)
	{
		mid = (e - s) / 2 + s;
		if (mid == definedcolors.size())
		{
			return Color(0.0f, 0.0f, 0.0f);
		}
		if (definedcolors[mid].name[0] == color[0])
		{
			found = true;
		}
		else if (definedcolors[mid].name[0] > color[0])
		{
			e = mid - 1;
		}
		else if (definedcolors[mid].name[0] < color[0])
		{
			s = mid + 1;
		}
		else if (mid == 0)
		{
			return Color(0.0f, 0.0f, 0.0f);
		}
	}

	if (definedcolors[mid].name == color)
	{
		return definedcolors[mid].color;
	}
	else
	{
		int pos = 1;
		int nc = 1;
		while (pos == 1)
		{
			if (nc == color.size() && nc < definedcolors[mid].name.size())
			{
				pos = 0;
			}
			else if (nc < color.size() && nc == definedcolors[mid].name.size())
			{
				pos = 2;
			}
			else
			{
				if (definedcolors[mid].name[1] == color[1])
				{
					nc = nc + 1;
				}
				else
				{
					if (definedcolors[mid].name[1] < color[1])
					{
						pos = 2;
					}
					else
					{
						pos = 0;
					}

				}
			}
		}
		if (pos == 0)
		{
			for (int i = mid - 1; i > 0; i--)
			{
				if (definedcolors[i].name == color)
				{
					return definedcolors[i].color;
				}
				else if (definedcolors[i].name[0] != color[0])
					return Color(0.0f, 0.0f, 0.0f);
			}
		}
		else
		{
			for (int i = mid + 1; i < definedcolors.size(); i++)
			{
				if (definedcolors[i].name == color)
				{
					return definedcolors[i].color;
				}
				else if (definedcolors[i].name[0] != color[0])
					return Color(0.0f, 0.0f, 0.0f);
			}
		}
	}
}

static void DefineColors(vector<DefinedColor>& definedcolors)
{
	definedcolors.push_back(DefinedColor("aliceblue", Color(0.941176, 0.972549, 1.000000)));
	definedcolors.push_back(DefinedColor("antiquewhite", Color(0.980392, 0.921569, 0.843137)));
	definedcolors.push_back(DefinedColor("antiquewhite1", Color(1.000000, 0.937255, 0.858824)));
	definedcolors.push_back(DefinedColor("antiquewhite2", Color(0.933333, 0.874510, 0.800000)));
	definedcolors.push_back(DefinedColor("antiquewhite3", Color(0.803922, 0.752941, 0.690196)));
	definedcolors.push_back(DefinedColor("antiquewhite4", Color(0.545098, 0.513726, 0.470588)));
	definedcolors.push_back(DefinedColor("aqua", Color(0.000000, 1.000000, 1.000000)));
	definedcolors.push_back(DefinedColor("aquamarine1", Color(0.498039, 1.000000, 0.831373)));
	definedcolors.push_back(DefinedColor("aquamarine2", Color(0.462745, 0.933333, 0.776471)));
	definedcolors.push_back(DefinedColor("aquamarine3", Color(0.400000, 0.803922, 0.666667)));
	definedcolors.push_back(DefinedColor("aquamarine4", Color(0.270588, 0.545098, 0.454902)));
	definedcolors.push_back(DefinedColor("azure1", Color(0.941176, 1.000000, 1.000000)));
	definedcolors.push_back(DefinedColor("azure2", Color(0.878431, 0.933333, 0.933333)));
	definedcolors.push_back(DefinedColor("azure3", Color(0.756863, 0.803922, 0.803922)));
	definedcolors.push_back(DefinedColor("azure4", Color(0.513726, 0.545098, 0.545098)));
	definedcolors.push_back(DefinedColor("banana", Color(0.890196, 0.811765, 0.341176)));
	definedcolors.push_back(DefinedColor("beige", Color(0.960784, 0.960784, 0.862745)));
	definedcolors.push_back(DefinedColor("bisque1", Color(1.000000, 0.894118, 0.768627)));
	definedcolors.push_back(DefinedColor("bisque2", Color(0.933333, 0.835294, 0.717647)));
	definedcolors.push_back(DefinedColor("bisque3", Color(0.803922, 0.717647, 0.619608)));
	definedcolors.push_back(DefinedColor("bisque4", Color(0.545098, 0.490196, 0.419608)));
	definedcolors.push_back(DefinedColor("black", Color(0.000000, 0.000000, 0.000000)));
	definedcolors.push_back(DefinedColor("blanchedalmond", Color(1.000000, 0.921569, 0.803922)));
	definedcolors.push_back(DefinedColor("blue", Color(0.000000, 0.000000, 1.000000)));
	definedcolors.push_back(DefinedColor("blue2", Color(0.000000, 0.000000, 0.933333)));
	definedcolors.push_back(DefinedColor("blue3", Color(0.000000, 0.000000, 0.803922)));
	definedcolors.push_back(DefinedColor("blue4", Color(0.000000, 0.000000, 0.545098)));
	definedcolors.push_back(DefinedColor("blueviolet", Color(0.541176, 0.168627, 0.886275)));
	definedcolors.push_back(DefinedColor("brick", Color(0.611765, 0.400000, 0.121569)));
	definedcolors.push_back(DefinedColor("brown", Color(0.647059, 0.164706, 0.164706)));
	definedcolors.push_back(DefinedColor("brown1", Color(1.000000, 0.250980, 0.250980)));
	definedcolors.push_back(DefinedColor("brown2", Color(0.933333, 0.231373, 0.231373)));
	definedcolors.push_back(DefinedColor("brown3", Color(0.803922, 0.200000, 0.200000)));
	definedcolors.push_back(DefinedColor("brown4", Color(0.545098, 0.137255, 0.137255)));
	definedcolors.push_back(DefinedColor("burlywood", Color(0.870588, 0.721569, 0.529412)));
	definedcolors.push_back(DefinedColor("burlywood1", Color(1.000000, 0.827451, 0.607843)));
	definedcolors.push_back(DefinedColor("burlywood2", Color(0.933333, 0.772549, 0.568627)));
	definedcolors.push_back(DefinedColor("burlywood3", Color(0.803922, 0.666667, 0.490196)));
	definedcolors.push_back(DefinedColor("burlywood4", Color(0.545098, 0.450980, 0.333333)));
	definedcolors.push_back(DefinedColor("burntsienna", Color(0.541176, 0.211765, 0.058824)));
	definedcolors.push_back(DefinedColor("burntumber", Color(0.541176, 0.200000, 0.141176)));
	definedcolors.push_back(DefinedColor("cadetblue", Color(0.372549, 0.619608, 0.627451)));
	definedcolors.push_back(DefinedColor("cadetblue1", Color(0.596078, 0.960784, 1.000000)));
	definedcolors.push_back(DefinedColor("cadetblue2", Color(0.556863, 0.898039, 0.933333)));
	definedcolors.push_back(DefinedColor("cadetblue3", Color(0.478431, 0.772549, 0.803922)));
	definedcolors.push_back(DefinedColor("cadetblue4", Color(0.325490, 0.525490, 0.545098)));
	definedcolors.push_back(DefinedColor("cadmiumorange", Color(1.000000, 0.380392, 0.011765)));
	definedcolors.push_back(DefinedColor("cadmiumyellow", Color(1.000000, 0.600000, 0.070588)));
	definedcolors.push_back(DefinedColor("carrot", Color(0.929412, 0.568627, 0.129412)));
	definedcolors.push_back(DefinedColor("chartreuse1", Color(0.498039, 1.000000, 0.000000)));
	definedcolors.push_back(DefinedColor("chartreuse2", Color(0.462745, 0.933333, 0.000000)));
	definedcolors.push_back(DefinedColor("chartreuse3", Color(0.400000, 0.803922, 0.000000)));
	definedcolors.push_back(DefinedColor("chartreuse4", Color(0.270588, 0.545098, 0.000000)));
	definedcolors.push_back(DefinedColor("chocolate", Color(0.823529, 0.411765, 0.117647)));
	definedcolors.push_back(DefinedColor("chocolate1", Color(1.000000, 0.498039, 0.141176)));
	definedcolors.push_back(DefinedColor("chocolate2", Color(0.933333, 0.462745, 0.129412)));
	definedcolors.push_back(DefinedColor("chocolate3", Color(0.803922, 0.400000, 0.113725)));
	definedcolors.push_back(DefinedColor("chocolate4", Color(0.545098, 0.270588, 0.074510)));
	definedcolors.push_back(DefinedColor("cobalt", Color(0.239216, 0.349020, 0.670588)));
	definedcolors.push_back(DefinedColor("cobaltgreen", Color(0.239216, 0.568627, 0.250980)));
	definedcolors.push_back(DefinedColor("coldgrey", Color(0.501961, 0.541176, 0.529412)));
	definedcolors.push_back(DefinedColor("coral", Color(1.000000, 0.498039, 0.313726)));
	definedcolors.push_back(DefinedColor("coral1", Color(1.000000, 0.447059, 0.337255)));
	definedcolors.push_back(DefinedColor("coral2", Color(0.933333, 0.415686, 0.313726)));
	definedcolors.push_back(DefinedColor("coral3", Color(0.803922, 0.356863, 0.270588)));
	definedcolors.push_back(DefinedColor("coral4", Color(0.545098, 0.243137, 0.184314)));
	definedcolors.push_back(DefinedColor("cornflowerblue", Color(0.392157, 0.584314, 0.929412)));
	definedcolors.push_back(DefinedColor("cornsilk1", Color(1.000000, 0.972549, 0.862745)));
	definedcolors.push_back(DefinedColor("cornsilk2", Color(0.933333, 0.909804, 0.803922)));
	definedcolors.push_back(DefinedColor("cornsilk3", Color(0.803922, 0.784314, 0.694118)));
	definedcolors.push_back(DefinedColor("cornsilk4", Color(0.545098, 0.533333, 0.470588)));
	definedcolors.push_back(DefinedColor("crimson", Color(0.862745, 0.078431, 0.235294)));
	definedcolors.push_back(DefinedColor("cyan2", Color(0.000000, 0.933333, 0.933333)));
	definedcolors.push_back(DefinedColor("cyan3", Color(0.000000, 0.803922, 0.803922)));
	definedcolors.push_back(DefinedColor("cyan4", Color(0.000000, 0.545098, 0.545098)));
	definedcolors.push_back(DefinedColor("darkgoldenrod", Color(0.721569, 0.525490, 0.043137)));
	definedcolors.push_back(DefinedColor("darkgoldenrod1", Color(1.000000, 0.725490, 0.058824)));
	definedcolors.push_back(DefinedColor("darkgoldenrod2", Color(0.933333, 0.678431, 0.054902)));
	definedcolors.push_back(DefinedColor("darkgoldenrod3", Color(0.803922, 0.584314, 0.047059)));
	definedcolors.push_back(DefinedColor("darkgoldenrod4", Color(0.545098, 0.396078, 0.031373)));
	definedcolors.push_back(DefinedColor("darkgray", Color(0.662745, 0.662745, 0.662745)));
	definedcolors.push_back(DefinedColor("darkgreen", Color(0.000000, 0.392157, 0.000000)));
	definedcolors.push_back(DefinedColor("darkkhaki", Color(0.741176, 0.717647, 0.419608)));
	definedcolors.push_back(DefinedColor("darkolivegreen", Color(0.333333, 0.419608, 0.184314)));
	definedcolors.push_back(DefinedColor("darkolivegreen1", Color(0.792157, 1.000000, 0.439216)));
	definedcolors.push_back(DefinedColor("darkolivegreen2", Color(0.737255, 0.933333, 0.407843)));
	definedcolors.push_back(DefinedColor("darkolivegreen3", Color(0.635294, 0.803922, 0.352941)));
	definedcolors.push_back(DefinedColor("darkolivegreen4", Color(0.431373, 0.545098, 0.239216)));
	definedcolors.push_back(DefinedColor("darkorange", Color(1.000000, 0.549020, 0.000000)));
	definedcolors.push_back(DefinedColor("darkorange1", Color(1.000000, 0.498039, 0.000000)));
	definedcolors.push_back(DefinedColor("darkorange2", Color(0.933333, 0.462745, 0.000000)));
	definedcolors.push_back(DefinedColor("darkorange3", Color(0.803922, 0.400000, 0.000000)));
	definedcolors.push_back(DefinedColor("darkorange4", Color(0.545098, 0.270588, 0.000000)));
	definedcolors.push_back(DefinedColor("darkorchid", Color(0.600000, 0.196078, 0.800000)));
	definedcolors.push_back(DefinedColor("darkorchid1", Color(0.749020, 0.243137, 1.000000)));
	definedcolors.push_back(DefinedColor("darkorchid2", Color(0.698039, 0.227451, 0.933333)));
	definedcolors.push_back(DefinedColor("darkorchid3", Color(0.603922, 0.196078, 0.803922)));
	definedcolors.push_back(DefinedColor("darkorchid4", Color(0.407843, 0.133333, 0.545098)));
	definedcolors.push_back(DefinedColor("darksalmon", Color(0.913725, 0.588235, 0.478431)));
	definedcolors.push_back(DefinedColor("darkseagreen", Color(0.560784, 0.737255, 0.560784)));
	definedcolors.push_back(DefinedColor("darkseagreen1", Color(0.756863, 1.000000, 0.756863)));
	definedcolors.push_back(DefinedColor("darkseagreen2", Color(0.705882, 0.933333, 0.705882)));
	definedcolors.push_back(DefinedColor("darkseagreen3", Color(0.607843, 0.803922, 0.607843)));
	definedcolors.push_back(DefinedColor("darkseagreen4", Color(0.411765, 0.545098, 0.411765)));
	definedcolors.push_back(DefinedColor("darkslateblue", Color(0.282353, 0.239216, 0.545098)));
	definedcolors.push_back(DefinedColor("darkslategray", Color(0.184314, 0.309804, 0.309804)));
	definedcolors.push_back(DefinedColor("darkslategray1", Color(0.592157, 1.000000, 1.000000)));
	definedcolors.push_back(DefinedColor("darkslategray2", Color(0.552941, 0.933333, 0.933333)));
	definedcolors.push_back(DefinedColor("darkslategray3", Color(0.474510, 0.803922, 0.803922)));
	definedcolors.push_back(DefinedColor("darkslategray4", Color(0.321569, 0.545098, 0.545098)));
	definedcolors.push_back(DefinedColor("darkturquoise", Color(0.000000, 0.807843, 0.819608)));
	definedcolors.push_back(DefinedColor("darkviolet", Color(0.580392, 0.000000, 0.827451)));
	definedcolors.push_back(DefinedColor("deeppink1", Color(1.000000, 0.078431, 0.576471)));
	definedcolors.push_back(DefinedColor("deeppink2", Color(0.933333, 0.070588, 0.537255)));
	definedcolors.push_back(DefinedColor("deeppink3", Color(0.803922, 0.062745, 0.462745)));
	definedcolors.push_back(DefinedColor("deeppink4", Color(0.545098, 0.039216, 0.313726)));
	definedcolors.push_back(DefinedColor("deepskyblue1", Color(0.000000, 0.749020, 1.000000)));
	definedcolors.push_back(DefinedColor("deepskyblue2", Color(0.000000, 0.698039, 0.933333)));
	definedcolors.push_back(DefinedColor("deepskyblue3", Color(0.000000, 0.603922, 0.803922)));
	definedcolors.push_back(DefinedColor("deepskyblue4", Color(0.000000, 0.407843, 0.545098)));
	definedcolors.push_back(DefinedColor("dimgray", Color(0.411765, 0.411765, 0.411765)));
	definedcolors.push_back(DefinedColor("dodgerblue1", Color(0.117647, 0.564706, 1.000000)));
	definedcolors.push_back(DefinedColor("dodgerblue2", Color(0.109804, 0.525490, 0.933333)));
	definedcolors.push_back(DefinedColor("dodgerblue3", Color(0.094118, 0.454902, 0.803922)));
	definedcolors.push_back(DefinedColor("dodgerblue4", Color(0.062745, 0.305882, 0.545098)));
	definedcolors.push_back(DefinedColor("eggshell", Color(0.988235, 0.901961, 0.788235)));
	definedcolors.push_back(DefinedColor("emeraldgreen", Color(0.000000, 0.788235, 0.341176)));
	definedcolors.push_back(DefinedColor("firebrick", Color(0.698039, 0.133333, 0.133333)));
	definedcolors.push_back(DefinedColor("firebrick1", Color(1.000000, 0.188235, 0.188235)));
	definedcolors.push_back(DefinedColor("firebrick2", Color(0.933333, 0.172549, 0.172549)));
	definedcolors.push_back(DefinedColor("firebrick3", Color(0.803922, 0.149020, 0.149020)));
	definedcolors.push_back(DefinedColor("firebrick4", Color(0.545098, 0.101961, 0.101961)));
	definedcolors.push_back(DefinedColor("flesh", Color(1.000000, 0.490196, 0.250980)));
	definedcolors.push_back(DefinedColor("floralwhite", Color(1.000000, 0.980392, 0.941176)));
	definedcolors.push_back(DefinedColor("forestgreen", Color(0.133333, 0.545098, 0.133333)));
	definedcolors.push_back(DefinedColor("gainsboro", Color(0.862745, 0.862745, 0.862745)));
	definedcolors.push_back(DefinedColor("ghostwhite", Color(0.972549, 0.972549, 1.000000)));
	definedcolors.push_back(DefinedColor("gold1", Color(1.000000, 0.843137, 0.000000)));
	definedcolors.push_back(DefinedColor("gold2", Color(0.933333, 0.788235, 0.000000)));
	definedcolors.push_back(DefinedColor("gold3", Color(0.803922, 0.678431, 0.000000)));
	definedcolors.push_back(DefinedColor("gold4", Color(0.545098, 0.458824, 0.000000)));
	definedcolors.push_back(DefinedColor("goldenrod", Color(0.854902, 0.647059, 0.125490)));
	definedcolors.push_back(DefinedColor("goldenrod1", Color(1.000000, 0.756863, 0.145098)));
	definedcolors.push_back(DefinedColor("goldenrod2", Color(0.933333, 0.705882, 0.133333)));
	definedcolors.push_back(DefinedColor("goldenrod3", Color(0.803922, 0.607843, 0.113725)));
	definedcolors.push_back(DefinedColor("goldenrod4", Color(0.545098, 0.411765, 0.078431)));
	definedcolors.push_back(DefinedColor("gray", Color(0.501961, 0.501961, 0.501961)));
	definedcolors.push_back(DefinedColor("gray1", Color(0.011765, 0.011765, 0.011765)));
	definedcolors.push_back(DefinedColor("gray10", Color(0.101961, 0.101961, 0.101961)));
	definedcolors.push_back(DefinedColor("gray11", Color(0.109804, 0.109804, 0.109804)));
	definedcolors.push_back(DefinedColor("gray12", Color(0.121569, 0.121569, 0.121569)));
	definedcolors.push_back(DefinedColor("gray13", Color(0.129412, 0.129412, 0.129412)));
	definedcolors.push_back(DefinedColor("gray14", Color(0.141176, 0.141176, 0.141176)));
	definedcolors.push_back(DefinedColor("gray15", Color(0.149020, 0.149020, 0.149020)));
	definedcolors.push_back(DefinedColor("gray16", Color(0.160784, 0.160784, 0.160784)));
	definedcolors.push_back(DefinedColor("gray17", Color(0.168627, 0.168627, 0.168627)));
	definedcolors.push_back(DefinedColor("gray18", Color(0.180392, 0.180392, 0.180392)));
	definedcolors.push_back(DefinedColor("gray19", Color(0.188235, 0.188235, 0.188235)));
	definedcolors.push_back(DefinedColor("gray2", Color(0.019608, 0.019608, 0.019608)));
	definedcolors.push_back(DefinedColor("gray20", Color(0.200000, 0.200000, 0.200000)));
	definedcolors.push_back(DefinedColor("gray21", Color(0.211765, 0.211765, 0.211765)));
	definedcolors.push_back(DefinedColor("gray22", Color(0.219608, 0.219608, 0.219608)));
	definedcolors.push_back(DefinedColor("gray23", Color(0.231373, 0.231373, 0.231373)));
	definedcolors.push_back(DefinedColor("gray24", Color(0.239216, 0.239216, 0.239216)));
	definedcolors.push_back(DefinedColor("gray25", Color(0.250980, 0.250980, 0.250980)));
	definedcolors.push_back(DefinedColor("gray26", Color(0.258824, 0.258824, 0.258824)));
	definedcolors.push_back(DefinedColor("gray27", Color(0.270588, 0.270588, 0.270588)));
	definedcolors.push_back(DefinedColor("gray28", Color(0.278431, 0.278431, 0.278431)));
	definedcolors.push_back(DefinedColor("gray29", Color(0.290196, 0.290196, 0.290196)));
	definedcolors.push_back(DefinedColor("gray3", Color(0.031373, 0.031373, 0.031373)));
	definedcolors.push_back(DefinedColor("gray30", Color(0.301961, 0.301961, 0.301961)));
	definedcolors.push_back(DefinedColor("gray31", Color(0.309804, 0.309804, 0.309804)));
	definedcolors.push_back(DefinedColor("gray32", Color(0.321569, 0.321569, 0.321569)));
	definedcolors.push_back(DefinedColor("gray33", Color(0.329412, 0.329412, 0.329412)));
	definedcolors.push_back(DefinedColor("gray34", Color(0.341176, 0.341176, 0.341176)));
	definedcolors.push_back(DefinedColor("gray35", Color(0.349020, 0.349020, 0.349020)));
	definedcolors.push_back(DefinedColor("gray36", Color(0.360784, 0.360784, 0.360784)));
	definedcolors.push_back(DefinedColor("gray37", Color(0.368627, 0.368627, 0.368627)));
	definedcolors.push_back(DefinedColor("gray38", Color(0.380392, 0.380392, 0.380392)));
	definedcolors.push_back(DefinedColor("gray39", Color(0.388235, 0.388235, 0.388235)));
	definedcolors.push_back(DefinedColor("gray4", Color(0.039216, 0.039216, 0.039216)));
	definedcolors.push_back(DefinedColor("gray40", Color(0.400000, 0.400000, 0.400000)));
	definedcolors.push_back(DefinedColor("gray42", Color(0.419608, 0.419608, 0.419608)));
	definedcolors.push_back(DefinedColor("gray43", Color(0.431373, 0.431373, 0.431373)));
	definedcolors.push_back(DefinedColor("gray44", Color(0.439216, 0.439216, 0.439216)));
	definedcolors.push_back(DefinedColor("gray45", Color(0.450980, 0.450980, 0.450980)));
	definedcolors.push_back(DefinedColor("gray46", Color(0.458824, 0.458824, 0.458824)));
	definedcolors.push_back(DefinedColor("gray47", Color(0.470588, 0.470588, 0.470588)));
	definedcolors.push_back(DefinedColor("gray48", Color(0.478431, 0.478431, 0.478431)));
	definedcolors.push_back(DefinedColor("gray49", Color(0.490196, 0.490196, 0.490196)));
	definedcolors.push_back(DefinedColor("gray5", Color(0.050980, 0.050980, 0.050980)));
	definedcolors.push_back(DefinedColor("gray50", Color(0.498039, 0.498039, 0.498039)));
	definedcolors.push_back(DefinedColor("gray51", Color(0.509804, 0.509804, 0.509804)));
	definedcolors.push_back(DefinedColor("gray52", Color(0.521569, 0.521569, 0.521569)));
	definedcolors.push_back(DefinedColor("gray53", Color(0.529412, 0.529412, 0.529412)));
	definedcolors.push_back(DefinedColor("gray54", Color(0.541176, 0.541176, 0.541176)));
	definedcolors.push_back(DefinedColor("gray55", Color(0.549020, 0.549020, 0.549020)));
	definedcolors.push_back(DefinedColor("gray56", Color(0.560784, 0.560784, 0.560784)));
	definedcolors.push_back(DefinedColor("gray57", Color(0.568627, 0.568627, 0.568627)));
	definedcolors.push_back(DefinedColor("gray58", Color(0.580392, 0.580392, 0.580392)));
	definedcolors.push_back(DefinedColor("gray59", Color(0.588235, 0.588235, 0.588235)));
	definedcolors.push_back(DefinedColor("gray6", Color(0.058824, 0.058824, 0.058824)));
	definedcolors.push_back(DefinedColor("gray60", Color(0.600000, 0.600000, 0.600000)));
	definedcolors.push_back(DefinedColor("gray61", Color(0.611765, 0.611765, 0.611765)));
	definedcolors.push_back(DefinedColor("gray62", Color(0.619608, 0.619608, 0.619608)));
	definedcolors.push_back(DefinedColor("gray63", Color(0.631373, 0.631373, 0.631373)));
	definedcolors.push_back(DefinedColor("gray64", Color(0.639216, 0.639216, 0.639216)));
	definedcolors.push_back(DefinedColor("gray65", Color(0.650980, 0.650980, 0.650980)));
	definedcolors.push_back(DefinedColor("gray66", Color(0.658824, 0.658824, 0.658824)));
	definedcolors.push_back(DefinedColor("gray67", Color(0.670588, 0.670588, 0.670588)));
	definedcolors.push_back(DefinedColor("gray68", Color(0.678431, 0.678431, 0.678431)));
	definedcolors.push_back(DefinedColor("gray69", Color(0.690196, 0.690196, 0.690196)));
	definedcolors.push_back(DefinedColor("gray7", Color(0.070588, 0.070588, 0.070588)));
	definedcolors.push_back(DefinedColor("gray70", Color(0.701961, 0.701961, 0.701961)));
	definedcolors.push_back(DefinedColor("gray71", Color(0.709804, 0.709804, 0.709804)));
	definedcolors.push_back(DefinedColor("gray72", Color(0.721569, 0.721569, 0.721569)));
	definedcolors.push_back(DefinedColor("gray73", Color(0.729412, 0.729412, 0.729412)));
	definedcolors.push_back(DefinedColor("gray74", Color(0.741176, 0.741176, 0.741176)));
	definedcolors.push_back(DefinedColor("gray75", Color(0.749020, 0.749020, 0.749020)));
	definedcolors.push_back(DefinedColor("gray76", Color(0.760784, 0.760784, 0.760784)));
	definedcolors.push_back(DefinedColor("gray77", Color(0.768627, 0.768627, 0.768627)));
	definedcolors.push_back(DefinedColor("gray78", Color(0.780392, 0.780392, 0.780392)));
	definedcolors.push_back(DefinedColor("gray79", Color(0.788235, 0.788235, 0.788235)));
	definedcolors.push_back(DefinedColor("gray8", Color(0.078431, 0.078431, 0.078431)));
	definedcolors.push_back(DefinedColor("gray80",           Color(0.800000, 0.800000, 0.800000)));
	definedcolors.push_back(DefinedColor("gray81",      Color(0.811765, 0.811765, 0.811765)));
	definedcolors.push_back(DefinedColor("gray82",      Color(0.819608, 0.819608, 0.819608)));
	definedcolors.push_back(DefinedColor("gray83",      Color(0.831373, 0.831373, 0.831373)));
	definedcolors.push_back(DefinedColor("gray84",      Color(0.839216, 0.839216, 0.839216)));
	definedcolors.push_back(DefinedColor("gray85",      Color(0.850980, 0.850980, 0.850980)));
	definedcolors.push_back(DefinedColor("gray86",      Color(0.858824, 0.858824, 0.858824)));
	definedcolors.push_back(DefinedColor("gray87",      Color(0.870588, 0.870588, 0.870588)));
	definedcolors.push_back(DefinedColor("gray88",      Color(0.878431, 0.878431, 0.878431)));
	definedcolors.push_back(DefinedColor("gray89",      Color(0.890196, 0.890196, 0.890196)));
	definedcolors.push_back(DefinedColor("gray9",       Color(0.090196, 0.090196, 0.090196)));
	definedcolors.push_back(DefinedColor("gray90",      Color(0.898039, 0.898039, 0.898039)));
	definedcolors.push_back(DefinedColor("gray91",      Color(0.909804, 0.909804, 0.909804)));
	definedcolors.push_back(DefinedColor("gray92",      Color(0.921569, 0.921569, 0.921569)));
	definedcolors.push_back(DefinedColor("gray93",      Color(0.929412, 0.929412, 0.929412)));
	definedcolors.push_back(DefinedColor("gray94",      Color(0.941176, 0.941176, 0.941176)));
	definedcolors.push_back(DefinedColor("gray95",      Color(0.949020, 0.949020, 0.949020)));
	definedcolors.push_back(DefinedColor("gray97",      Color(0.968627, 0.968627, 0.968627)));
	definedcolors.push_back(DefinedColor("gray98",      Color(0.980392, 0.980392, 0.980392)));
	definedcolors.push_back(DefinedColor("gray99",      Color(0.988235, 0.988235, 0.988235)));
	definedcolors.push_back(DefinedColor("green",       Color(0.000000, 0.501961, 0.000000)));
	definedcolors.push_back(DefinedColor("green1",      Color(0.000000, 1.000000, 0.000000)));
	definedcolors.push_back(DefinedColor("green2",      Color(0.000000, 0.933333, 0.000000)));
	definedcolors.push_back(DefinedColor("green3",      Color(0.000000, 0.803922, 0.000000)));
	definedcolors.push_back(DefinedColor("green4",      Color(0.000000, 0.545098, 0.000000)));
	definedcolors.push_back(DefinedColor("greenyellow", Color(0.678431, 1.000000, 0.184314)));
	definedcolors.push_back(DefinedColor("honeydew1", Color(0.941176, 1.000000, 0.941176)));
	definedcolors.push_back(DefinedColor("honeydew2", Color(0.878431, 0.933333, 0.878431)));
	definedcolors.push_back(DefinedColor("honeydew3", Color(0.756863, 0.803922, 0.756863)));
	definedcolors.push_back(DefinedColor("honeydew4", Color(0.513726, 0.545098, 0.513726)));
	definedcolors.push_back(DefinedColor("hotpink", Color(1.000000, 0.411765, 0.705882)));
	definedcolors.push_back(DefinedColor("hotpink1", Color(1.000000, 0.431373, 0.705882)));
	definedcolors.push_back(DefinedColor("hotpink2", Color(0.933333, 0.415686, 0.654902)));
	definedcolors.push_back(DefinedColor("hotpink3", Color(0.803922, 0.376471, 0.564706)));
	definedcolors.push_back(DefinedColor("hotpink4", Color(0.545098, 0.227451, 0.384314)));
	definedcolors.push_back(DefinedColor("indianred", Color(0.803922, 0.360784, 0.360784)));
	definedcolors.push_back(DefinedColor("indianred1", Color(1.000000, 0.415686, 0.415686)));
	definedcolors.push_back(DefinedColor("indianred2", Color(0.933333, 0.388235, 0.388235)));
	definedcolors.push_back(DefinedColor("indianred3", Color(0.803922, 0.333333, 0.333333)));
	definedcolors.push_back(DefinedColor("indianred4", Color(0.545098, 0.227451, 0.227451)));
	definedcolors.push_back(DefinedColor("indigo", Color(0.294118, 0.000000, 0.509804)));
	definedcolors.push_back(DefinedColor("ivory1", Color(1.000000, 1.000000, 0.941176)));
	definedcolors.push_back(DefinedColor("ivory2", Color(0.933333, 0.933333, 0.878431)));
	definedcolors.push_back(DefinedColor("ivory3", Color(0.803922, 0.803922, 0.756863)));
	definedcolors.push_back(DefinedColor("ivory4", Color(0.545098, 0.545098, 0.513726)));
	definedcolors.push_back(DefinedColor("ivoryblack", Color(0.160784, 0.141176, 0.129412)));
	definedcolors.push_back(DefinedColor("khaki", Color(0.941176, 0.901961, 0.549020)));
	definedcolors.push_back(DefinedColor("khaki1", Color(1.000000, 0.964706, 0.560784)));
	definedcolors.push_back(DefinedColor("khaki2", Color(0.933333, 0.901961, 0.521569)));
	definedcolors.push_back(DefinedColor("khaki3", Color(0.803922, 0.776471, 0.450980)));
	definedcolors.push_back(DefinedColor("khaki4", Color(0.545098, 0.525490, 0.305882)));
	definedcolors.push_back(DefinedColor("lavender", Color(0.901961, 0.901961, 0.980392)));
	definedcolors.push_back(DefinedColor("lavenderblush1", Color(1.000000, 0.941176, 0.960784)));
	definedcolors.push_back(DefinedColor("lavenderblush2", Color(0.933333, 0.878431, 0.898039)));
	definedcolors.push_back(DefinedColor("lavenderblush3", Color(0.803922, 0.756863, 0.772549)));
	definedcolors.push_back(DefinedColor("lavenderblush4", Color(0.545098, 0.513726, 0.525490)));
	definedcolors.push_back(DefinedColor("lawngreen", Color(0.486275, 0.988235, 0.000000)));
	definedcolors.push_back(DefinedColor("lemonchiffon1", Color(1.000000, 0.980392, 0.803922)));
	definedcolors.push_back(DefinedColor("lemonchiffon2", Color(0.933333, 0.913725, 0.749020)));
	definedcolors.push_back(DefinedColor("lemonchiffon3", Color(0.803922, 0.788235, 0.647059)));
	definedcolors.push_back(DefinedColor("lemonchiffon4", Color(0.545098, 0.537255, 0.439216)));
	definedcolors.push_back(DefinedColor("lightblue", Color(0.678431, 0.847059, 0.901961)));
	definedcolors.push_back(DefinedColor("lightblue1", Color(0.749020, 0.937255, 1.000000)));
	definedcolors.push_back(DefinedColor("lightblue2", Color(0.698039, 0.874510, 0.933333)));
	definedcolors.push_back(DefinedColor("lightblue3", Color(0.603922, 0.752941, 0.803922)));
	definedcolors.push_back(DefinedColor("lightblue4", Color(0.407843, 0.513726, 0.545098)));
	definedcolors.push_back(DefinedColor("lightcoral", Color(0.941176, 0.501961, 0.501961)));
	definedcolors.push_back(DefinedColor("lightcyan1", Color(0.878431, 1.000000, 1.000000)));
	definedcolors.push_back(DefinedColor("lightcyan2", Color(0.819608, 0.933333, 0.933333)));
	definedcolors.push_back(DefinedColor("lightcyan3", Color(0.705882, 0.803922, 0.803922)));
	definedcolors.push_back(DefinedColor("lightcyan4", Color(0.478431, 0.545098, 0.545098)));
	definedcolors.push_back(DefinedColor("lightgoldenrod1", Color(1.000000, 0.925490, 0.545098)));
	definedcolors.push_back(DefinedColor("lightgoldenrod2", Color(0.933333, 0.862745, 0.509804)));
	definedcolors.push_back(DefinedColor("lightgoldenrod3", Color(0.803922, 0.745098, 0.439216)));
	definedcolors.push_back(DefinedColor("lightgoldenrod4", Color(0.545098, 0.505882, 0.298039)));
	definedcolors.push_back(DefinedColor("lightgoldenrodyellow", Color(0.980392, 0.980392, 0.823529)));
	definedcolors.push_back(DefinedColor("lightgrey", Color(0.827451, 0.827451, 0.827451)));
	definedcolors.push_back(DefinedColor("lightpink", Color(1.000000, 0.713726, 0.756863)));
	definedcolors.push_back(DefinedColor("lightpink1", Color(1.000000, 0.682353, 0.725490)));
	definedcolors.push_back(DefinedColor("lightpink2", Color(0.933333, 0.635294, 0.678431)));
	definedcolors.push_back(DefinedColor("lightpink3", Color(0.803922, 0.549020, 0.584314)));
	definedcolors.push_back(DefinedColor("lightpink4", Color(0.545098, 0.372549, 0.396078)));
	definedcolors.push_back(DefinedColor("lightsalmon1", Color(1.000000, 0.627451, 0.478431)));
	definedcolors.push_back(DefinedColor("lightsalmon2", Color(0.933333, 0.584314, 0.447059)));
	definedcolors.push_back(DefinedColor("lightsalmon3", Color(0.803922, 0.505882, 0.384314)));
	definedcolors.push_back(DefinedColor("lightsalmon4", Color(0.545098, 0.341176, 0.258824)));
	definedcolors.push_back(DefinedColor("lightseagreen", Color(0.125490, 0.698039, 0.666667)));
	definedcolors.push_back(DefinedColor("lightskyblue", Color(0.529412, 0.807843, 0.980392)));
	definedcolors.push_back(DefinedColor("lightskyblue1", Color(0.690196, 0.886275, 1.000000)));
	definedcolors.push_back(DefinedColor("lightskyblue2", Color(0.643137, 0.827451, 0.933333)));
	definedcolors.push_back(DefinedColor("lightskyblue3", Color(0.552941, 0.713726, 0.803922)));
	definedcolors.push_back(DefinedColor("lightskyblue4", Color(0.376471, 0.482353, 0.545098)));
	definedcolors.push_back(DefinedColor("lightslateblue", Color(0.517647, 0.439216, 1.000000)));
	definedcolors.push_back(DefinedColor("lightslategray", Color(0.466667, 0.533333, 0.600000)));
	definedcolors.push_back(DefinedColor("lightsteelblue", Color(0.690196, 0.768627, 0.870588)));
	definedcolors.push_back(DefinedColor("lightsteelblue1", Color(0.792157, 0.882353, 1.000000)));
	definedcolors.push_back(DefinedColor("lightsteelblue2", Color(0.737255, 0.823529, 0.933333)));
	definedcolors.push_back(DefinedColor("lightsteelblue3", Color(0.635294, 0.709804, 0.803922)));
	definedcolors.push_back(DefinedColor("lightsteelblue4", Color(0.431373, 0.482353, 0.545098)));
	definedcolors.push_back(DefinedColor("lightyellow1", Color(1.000000, 1.000000, 0.878431)));
	definedcolors.push_back(DefinedColor("lightyellow2", Color(0.933333, 0.933333, 0.819608)));
	definedcolors.push_back(DefinedColor("lightyellow3", Color(0.803922, 0.803922, 0.705882)));
	definedcolors.push_back(DefinedColor("lightyellow4", Color(0.545098, 0.545098, 0.478431)));
	definedcolors.push_back(DefinedColor("limegreen", Color(0.196078, 0.803922, 0.196078)));
	definedcolors.push_back(DefinedColor("linen", Color(0.980392, 0.941176, 0.901961)));
	definedcolors.push_back(DefinedColor("magenta", Color(1.000000, 0.000000, 1.000000)));
	definedcolors.push_back(DefinedColor("magenta2", Color(0.933333, 0.000000, 0.933333)));
	definedcolors.push_back(DefinedColor("magenta3", Color(0.803922, 0.000000, 0.803922)));
	definedcolors.push_back(DefinedColor("magenta4", Color(0.545098, 0.000000, 0.545098)));
	definedcolors.push_back(DefinedColor("manganeseblue", Color(0.011765, 0.658824, 0.619608)));
	definedcolors.push_back(DefinedColor("maroon", Color(0.501961, 0.000000, 0.000000)));
	definedcolors.push_back(DefinedColor("maroon1", Color(1.000000, 0.203922, 0.701961)));
	definedcolors.push_back(DefinedColor("maroon2", Color(0.933333, 0.188235, 0.654902)));
	definedcolors.push_back(DefinedColor("maroon3", Color(0.803922, 0.160784, 0.564706)));
	definedcolors.push_back(DefinedColor("maroon4", Color(0.545098, 0.109804, 0.384314)));
	definedcolors.push_back(DefinedColor("mediumorchid", Color(0.729412, 0.333333, 0.827451)));
	definedcolors.push_back(DefinedColor("mediumorchid1", Color(0.878431, 0.400000, 1.000000)));
	definedcolors.push_back(DefinedColor("mediumorchid2", Color(0.819608, 0.372549, 0.933333)));
	definedcolors.push_back(DefinedColor("mediumorchid3", Color(0.705882, 0.321569, 0.803922)));
	definedcolors.push_back(DefinedColor("mediumorchid4", Color(0.478431, 0.215686, 0.545098)));
	definedcolors.push_back(DefinedColor("mediumpurple", Color(0.576471, 0.439216, 0.858824)));
	definedcolors.push_back(DefinedColor("mediumpurple1", Color(0.670588, 0.509804, 1.000000)));
	definedcolors.push_back(DefinedColor("mediumpurple2", Color(0.623529, 0.474510, 0.933333)));
	definedcolors.push_back(DefinedColor("mediumpurple3", Color(0.537255, 0.407843, 0.803922)));
	definedcolors.push_back(DefinedColor("mediumpurple4", Color(0.364706, 0.278431, 0.545098)));
	definedcolors.push_back(DefinedColor("mediumseagreen", Color(0.235294, 0.701961, 0.443137)));
	definedcolors.push_back(DefinedColor("mediumslateblue", Color(0.482353, 0.407843, 0.933333)));
	definedcolors.push_back(DefinedColor("mediumspringgreen", Color(0.000000, 0.980392, 0.603922)));
	definedcolors.push_back(DefinedColor("mediumturquoise", Color(0.282353, 0.819608, 0.800000)));
	definedcolors.push_back(DefinedColor("mediumvioletred", Color(0.780392, 0.082353, 0.521569)));
	definedcolors.push_back(DefinedColor("melon", Color(0.890196, 0.658824, 0.411765)));
	definedcolors.push_back(DefinedColor("midnightblue", Color(0.098039, 0.098039, 0.439216)));
	definedcolors.push_back(DefinedColor("mint", Color(0.741176, 0.988235, 0.788235)));
	definedcolors.push_back(DefinedColor("mintcream", Color(0.960784, 1.000000, 0.980392)));
	definedcolors.push_back(DefinedColor("mistyrose1", Color(1.000000, 0.894118, 0.882353)));
	definedcolors.push_back(DefinedColor("mistyrose2", Color(0.933333, 0.835294, 0.823529)));
	definedcolors.push_back(DefinedColor("mistyrose3", Color(0.803922, 0.717647, 0.709804)));
	definedcolors.push_back(DefinedColor("mistyrose4", Color(0.545098, 0.490196, 0.482353)));
	definedcolors.push_back(DefinedColor("moccasin", Color(1.000000, 0.894118, 0.709804)));
	definedcolors.push_back(DefinedColor("navajowhite1", Color(1.000000, 0.870588, 0.678431)));
	definedcolors.push_back(DefinedColor("navajowhite2", Color(0.933333, 0.811765, 0.631373)));
	definedcolors.push_back(DefinedColor("navajowhite3", Color(0.803922, 0.701961, 0.545098)));
	definedcolors.push_back(DefinedColor("navajowhite4", Color(0.545098, 0.474510, 0.368627)));
	definedcolors.push_back(DefinedColor("navy", Color(0.000000, 0.000000, 0.501961)));
	definedcolors.push_back(DefinedColor("oldlace", Color(0.992157, 0.960784, 0.901961)));
	definedcolors.push_back(DefinedColor("olive", Color(0.501961, 0.501961, 0.000000)));
	definedcolors.push_back(DefinedColor("olivedrab", Color(0.419608, 0.556863, 0.137255)));
	definedcolors.push_back(DefinedColor("olivedrab1", Color(0.752941, 1.000000, 0.243137)));
	definedcolors.push_back(DefinedColor("olivedrab2", Color(0.701961, 0.933333, 0.227451)));
	definedcolors.push_back(DefinedColor("olivedrab3", Color(0.603922, 0.803922, 0.196078)));
	definedcolors.push_back(DefinedColor("olivedrab4", Color(0.411765, 0.545098, 0.133333)));
	definedcolors.push_back(DefinedColor("orange", Color(1.000000, 0.501961, 0.000000)));
	definedcolors.push_back(DefinedColor("orange1", Color(1.000000, 0.647059, 0.000000)));
	definedcolors.push_back(DefinedColor("orange2", Color(0.933333, 0.603922, 0.000000)));
	definedcolors.push_back(DefinedColor("orange3", Color(0.803922, 0.521569, 0.000000)));
	definedcolors.push_back(DefinedColor("orange4", Color(0.545098, 0.352941, 0.000000)));
	definedcolors.push_back(DefinedColor("orangered1", Color(1.000000, 0.270588, 0.000000)));
	definedcolors.push_back(DefinedColor("orangered2", Color(0.933333, 0.250980, 0.000000)));
	definedcolors.push_back(DefinedColor("orangered3", Color(0.803922, 0.215686, 0.000000)));
	definedcolors.push_back(DefinedColor("orangered4", Color(0.545098, 0.145098, 0.000000)));
	definedcolors.push_back(DefinedColor("orchid", Color(0.854902, 0.439216, 0.839216)));
	definedcolors.push_back(DefinedColor("orchid1", Color(1.000000, 0.513726, 0.980392)));
	definedcolors.push_back(DefinedColor("orchid2", Color(0.933333, 0.478431, 0.913725)));
	definedcolors.push_back(DefinedColor("orchid3", Color(0.803922, 0.411765, 0.788235)));
	definedcolors.push_back(DefinedColor("orchid4", Color(0.545098, 0.278431, 0.537255)));
	definedcolors.push_back(DefinedColor("palegoldenrod", Color(0.933333, 0.909804, 0.666667)));
	definedcolors.push_back(DefinedColor("palegreen", Color(0.596078, 0.984314, 0.596078)));
	definedcolors.push_back(DefinedColor("palegreen1", Color(0.603922, 1.000000, 0.603922)));
	definedcolors.push_back(DefinedColor("palegreen2", Color(0.564706, 0.933333, 0.564706)));
	definedcolors.push_back(DefinedColor("palegreen3", Color(0.486275, 0.803922, 0.486275)));
	definedcolors.push_back(DefinedColor("palegreen4", Color(0.329412, 0.545098, 0.329412)));
	definedcolors.push_back(DefinedColor("paleturquoise1", Color(0.733333, 1.000000, 1.000000)));
	definedcolors.push_back(DefinedColor("paleturquoise2", Color(0.682353, 0.933333, 0.933333)));
	definedcolors.push_back(DefinedColor("paleturquoise3", Color(0.588235, 0.803922, 0.803922)));
	definedcolors.push_back(DefinedColor("paleturquoise4", Color(0.400000, 0.545098, 0.545098)));
	definedcolors.push_back(DefinedColor("palevioletred", Color(0.858824, 0.439216, 0.576471)));
	definedcolors.push_back(DefinedColor("palevioletred1", Color(1.000000, 0.509804, 0.670588)));
	definedcolors.push_back(DefinedColor("palevioletred2", Color(0.933333, 0.474510, 0.623529)));
	definedcolors.push_back(DefinedColor("palevioletred3", Color(0.803922, 0.407843, 0.537255)));
	definedcolors.push_back(DefinedColor("palevioletred4", Color(0.545098, 0.278431, 0.364706)));
	definedcolors.push_back(DefinedColor("papayawhip", Color(1.000000, 0.937255, 0.835294)));
	definedcolors.push_back(DefinedColor("peachpuff1", Color(1.000000, 0.854902, 0.725490)));
	definedcolors.push_back(DefinedColor("peachpuff2", Color(0.933333, 0.796078, 0.678431)));
	definedcolors.push_back(DefinedColor("peachpuff3", Color(0.803922, 0.686275, 0.584314)));
	definedcolors.push_back(DefinedColor("peachpuff4", Color(0.545098, 0.466667, 0.396078)));
	definedcolors.push_back(DefinedColor("peacock", Color(0.200000, 0.631373, 0.788235)));
	definedcolors.push_back(DefinedColor("pink", Color(1.000000, 0.752941, 0.796078)));
	definedcolors.push_back(DefinedColor("pink1", Color(1.000000, 0.709804, 0.772549)));
	definedcolors.push_back(DefinedColor("pink2", Color(0.933333, 0.662745, 0.721569)));
	definedcolors.push_back(DefinedColor("pink3", Color(0.803922, 0.568627, 0.619608)));
	definedcolors.push_back(DefinedColor("pink4", Color(0.545098, 0.388235, 0.423529)));
	definedcolors.push_back(DefinedColor("plum", Color(0.866667, 0.627451, 0.866667)));
	definedcolors.push_back(DefinedColor("plum1", Color(1.000000, 0.733333, 1.000000)));
	definedcolors.push_back(DefinedColor("plum2", Color(0.933333, 0.682353, 0.933333)));
	definedcolors.push_back(DefinedColor("plum3", Color(0.803922, 0.588235, 0.803922)));
	definedcolors.push_back(DefinedColor("plum4", Color(0.545098, 0.400000, 0.545098)));
	definedcolors.push_back(DefinedColor("powderblue", Color(0.690196, 0.878431, 0.901961)));
	definedcolors.push_back(DefinedColor("purple", Color(0.501961, 0.000000, 0.501961)));
	definedcolors.push_back(DefinedColor("purple1", Color(0.607843, 0.188235, 1.000000)));
	definedcolors.push_back(DefinedColor("purple2", Color(0.568627, 0.172549, 0.933333)));
	definedcolors.push_back(DefinedColor("purple3", Color(0.490196, 0.149020, 0.803922)));
	definedcolors.push_back(DefinedColor("purple4", Color(0.333333, 0.101961, 0.545098)));
	definedcolors.push_back(DefinedColor("raspberry", Color(0.529412, 0.149020, 0.341176)));
	definedcolors.push_back(DefinedColor("rawsienna", Color(0.780392, 0.380392, 0.078431)));
	definedcolors.push_back(DefinedColor("red1", Color(1.000000, 0.000000, 0.000000)));
	definedcolors.push_back(DefinedColor("red2", Color(0.933333, 0.000000, 0.000000)));
	definedcolors.push_back(DefinedColor("red3", Color(0.803922, 0.000000, 0.000000)));
	definedcolors.push_back(DefinedColor("red4", Color(0.545098, 0.000000, 0.000000)));
	definedcolors.push_back(DefinedColor("rosybrown", Color(0.737255, 0.560784, 0.560784)));
	definedcolors.push_back(DefinedColor("rosybrown1", Color(1.000000, 0.756863, 0.756863)));
	definedcolors.push_back(DefinedColor("rosybrown2", Color(0.933333, 0.705882, 0.705882)));
	definedcolors.push_back(DefinedColor("rosybrown3", Color(0.803922, 0.607843, 0.607843)));
	definedcolors.push_back(DefinedColor("rosybrown4", Color(0.545098, 0.411765, 0.411765)));
	definedcolors.push_back(DefinedColor("royalblue", Color(0.254902, 0.411765, 0.882353)));
	definedcolors.push_back(DefinedColor("royalblue1", Color(0.282353, 0.462745, 1.000000)));
	definedcolors.push_back(DefinedColor("royalblue2", Color(0.262745, 0.431373, 0.933333)));
	definedcolors.push_back(DefinedColor("royalblue3", Color(0.227451, 0.372549, 0.803922)));
	definedcolors.push_back(DefinedColor("royalblue4", Color(0.152941, 0.250980, 0.545098)));
	definedcolors.push_back(DefinedColor("salmon", Color(0.980392, 0.501961, 0.447059)));
	definedcolors.push_back(DefinedColor("salmon1", Color(1.000000, 0.549020, 0.411765)));
	definedcolors.push_back(DefinedColor("salmon2", Color(0.933333, 0.509804, 0.384314)));
	definedcolors.push_back(DefinedColor("salmon3", Color(0.803922, 0.439216, 0.329412)));
	definedcolors.push_back(DefinedColor("salmon4", Color(0.545098, 0.298039, 0.223529)));
	definedcolors.push_back(DefinedColor("sandybrown", Color(0.956863, 0.643137, 0.376471)));
	definedcolors.push_back(DefinedColor("sapgreen", Color(0.188235, 0.501961, 0.078431)));
	definedcolors.push_back(DefinedColor("seagreen1", Color(0.329412, 1.000000, 0.623529)));
	definedcolors.push_back(DefinedColor("seagreen2", Color(0.305882, 0.933333, 0.580392)));
	definedcolors.push_back(DefinedColor("seagreen3", Color(0.262745, 0.803922, 0.501961)));
	definedcolors.push_back(DefinedColor("seagreen4", Color(0.180392, 0.545098, 0.341176)));
	definedcolors.push_back(DefinedColor("seashell1", Color(1.000000, 0.960784, 0.933333)));
	definedcolors.push_back(DefinedColor("seashell2", Color(0.933333, 0.898039, 0.870588)));
	definedcolors.push_back(DefinedColor("seashell3", Color(0.803922, 0.772549, 0.749020)));
	definedcolors.push_back(DefinedColor("seashell4", Color(0.545098, 0.525490, 0.509804)));
	definedcolors.push_back(DefinedColor("sepia", Color(0.368627, 0.149020, 0.070588)));
	definedcolors.push_back(DefinedColor("sgibeet", Color(0.556863, 0.219608, 0.556863)));
	definedcolors.push_back(DefinedColor("sgibrightgray", Color(0.772549, 0.756863, 0.666667)));
	definedcolors.push_back(DefinedColor("sgichartreuse", Color(0.443137, 0.776471, 0.443137)));
	definedcolors.push_back(DefinedColor("sgidarkgray", Color(0.333333, 0.333333, 0.333333)));
	definedcolors.push_back(DefinedColor("sgigray12", Color(0.117647, 0.117647, 0.117647)));
	definedcolors.push_back(DefinedColor("sgigray16", Color(0.156863, 0.156863, 0.156863)));
	definedcolors.push_back(DefinedColor("sgigray32", Color(0.317647, 0.317647, 0.317647)));
	definedcolors.push_back(DefinedColor("sgigray36", Color(0.356863, 0.356863, 0.356863)));
	definedcolors.push_back(DefinedColor("sgigray52", Color(0.517647, 0.517647, 0.517647)));
	definedcolors.push_back(DefinedColor("sgigray56", Color(0.556863, 0.556863, 0.556863)));
	definedcolors.push_back(DefinedColor("sgigray72", Color(0.717647, 0.717647, 0.717647)));
	definedcolors.push_back(DefinedColor("sgigray76", Color(0.756863, 0.756863, 0.756863)));
	definedcolors.push_back(DefinedColor("sgigray92", Color(0.917647, 0.917647, 0.917647)));
	definedcolors.push_back(DefinedColor("sgigray96", Color(0.956863, 0.956863, 0.956863)));
	definedcolors.push_back(DefinedColor("sgilightblue", Color(0.490196, 0.619608, 0.752941)));
	definedcolors.push_back(DefinedColor("sgilightgray", Color(0.666667, 0.666667, 0.666667)));
	definedcolors.push_back(DefinedColor("sgiolivedrab", Color(0.556863, 0.556863, 0.219608)));
	definedcolors.push_back(DefinedColor("sgisalmon", Color(0.776471, 0.443137, 0.443137)));
	definedcolors.push_back(DefinedColor("sgislateblue", Color(0.443137, 0.443137, 0.776471)));
	definedcolors.push_back(DefinedColor("sgiteal", Color(0.219608, 0.556863, 0.556863)));
	definedcolors.push_back(DefinedColor("sienna", Color(0.627451, 0.321569, 0.176471)));
	definedcolors.push_back(DefinedColor("sienna1", Color(1.000000, 0.509804, 0.278431)));
	definedcolors.push_back(DefinedColor("sienna2", Color(0.933333, 0.474510, 0.258824)));
	definedcolors.push_back(DefinedColor("sienna3", Color(0.803922, 0.407843, 0.223529)));
	definedcolors.push_back(DefinedColor("sienna4", Color(0.545098, 0.278431, 0.149020)));
	definedcolors.push_back(DefinedColor("silver", Color(0.752941, 0.752941, 0.752941)));
	definedcolors.push_back(DefinedColor("skyblue", Color(0.529412, 0.807843, 0.921569)));
	definedcolors.push_back(DefinedColor("skyblue1", Color(0.529412, 0.807843, 1.000000)));
	definedcolors.push_back(DefinedColor("skyblue2", Color(0.494118, 0.752941, 0.933333)));
	definedcolors.push_back(DefinedColor("skyblue3", Color(0.423529, 0.650980, 0.803922)));
	definedcolors.push_back(DefinedColor("skyblue4", Color(0.290196, 0.439216, 0.545098)));
	definedcolors.push_back(DefinedColor("slateblue", Color(0.415686, 0.352941, 0.803922)));
	definedcolors.push_back(DefinedColor("slateblue1", Color(0.513726, 0.435294, 1.000000)));
	definedcolors.push_back(DefinedColor("slateblue2", Color(0.478431, 0.403922, 0.933333)));
	definedcolors.push_back(DefinedColor("slateblue3", Color(0.411765, 0.349020, 0.803922)));
	definedcolors.push_back(DefinedColor("slateblue4", Color(0.278431, 0.235294, 0.545098)));
	definedcolors.push_back(DefinedColor("slategray", Color(0.439216, 0.501961, 0.564706)));
	definedcolors.push_back(DefinedColor("slategray1", Color(0.776471, 0.886275, 1.000000)));
	definedcolors.push_back(DefinedColor("slategray2", Color(0.725490, 0.827451, 0.933333)));
	definedcolors.push_back(DefinedColor("slategray3", Color(0.623529, 0.713726, 0.803922)));
	definedcolors.push_back(DefinedColor("slategray4", Color(0.423529, 0.482353, 0.545098)));
	definedcolors.push_back(DefinedColor("snow1", Color(1.000000, 0.980392, 0.980392)));
	definedcolors.push_back(DefinedColor("snow2", Color(0.933333, 0.913725, 0.913725)));
	definedcolors.push_back(DefinedColor("snow3", Color(0.803922, 0.788235, 0.788235)));
	definedcolors.push_back(DefinedColor("snow4", Color(0.545098, 0.537255, 0.537255)));
	definedcolors.push_back(DefinedColor("springgreen", Color(0.000000, 1.000000, 0.498039)));
	definedcolors.push_back(DefinedColor("springgreen1", Color(0.000000, 0.933333, 0.462745)));
	definedcolors.push_back(DefinedColor("springgreen2", Color(0.000000, 0.803922, 0.400000)));
	definedcolors.push_back(DefinedColor("springgreen3", Color(0.000000, 0.545098, 0.270588)));
	definedcolors.push_back(DefinedColor("steelblue", Color(0.274510, 0.509804, 0.705882)));
	definedcolors.push_back(DefinedColor("steelblue1", Color(0.388235, 0.721569, 1.000000)));
	definedcolors.push_back(DefinedColor("steelblue2", Color(0.360784, 0.674510, 0.933333)));
	definedcolors.push_back(DefinedColor("steelblue3", Color(0.309804, 0.580392, 0.803922)));
	definedcolors.push_back(DefinedColor("steelblue4", Color(0.211765, 0.392157, 0.545098)));
	definedcolors.push_back(DefinedColor("tan", Color(0.823529, 0.705882, 0.549020)));
	definedcolors.push_back(DefinedColor("tan1", Color(1.000000, 0.647059, 0.309804)));
	definedcolors.push_back(DefinedColor("tan2", Color(0.933333, 0.603922, 0.286275)));
	definedcolors.push_back(DefinedColor("tan3", Color(0.803922, 0.521569, 0.247059)));
	definedcolors.push_back(DefinedColor("tan4", Color(0.545098, 0.352941, 0.168627)));
	definedcolors.push_back(DefinedColor("teal", Color(0.000000, 0.501961, 0.501961)));
	definedcolors.push_back(DefinedColor("thistle", Color(0.847059, 0.749020, 0.847059)));
	definedcolors.push_back(DefinedColor("thistle1", Color(1.000000, 0.882353, 1.000000)));
	definedcolors.push_back(DefinedColor("thistle2", Color(0.933333, 0.823529, 0.933333)));
	definedcolors.push_back(DefinedColor("thistle3", Color(0.803922, 0.709804, 0.803922)));
	definedcolors.push_back(DefinedColor("thistle4", Color(0.545098, 0.482353, 0.545098)));
	definedcolors.push_back(DefinedColor("tomato1", Color(1.000000, 0.388235, 0.278431)));
	definedcolors.push_back(DefinedColor("tomato2", Color(0.933333, 0.360784, 0.258824)));
	definedcolors.push_back(DefinedColor("tomato3", Color(0.803922, 0.309804, 0.223529)));
	definedcolors.push_back(DefinedColor("tomato4", Color(0.545098, 0.211765, 0.149020)));
	definedcolors.push_back(DefinedColor("turquoise", Color(0.250980, 0.878431, 0.815686)));
	definedcolors.push_back(DefinedColor("turquoise1", Color(0.000000, 0.960784, 1.000000)));
	definedcolors.push_back(DefinedColor("turquoise2", Color(0.000000, 0.898039, 0.933333)));
	definedcolors.push_back(DefinedColor("turquoise3", Color(0.000000, 0.772549, 0.803922)));
	definedcolors.push_back(DefinedColor("turquoise4", Color(0.000000, 0.525490, 0.545098)));
	definedcolors.push_back(DefinedColor("turquoiseblue", Color(0.000000, 0.780392, 0.549020)));
	definedcolors.push_back(DefinedColor("violet", Color(0.933333, 0.509804, 0.933333)));
	definedcolors.push_back(DefinedColor("violetred", Color(0.815686, 0.125490, 0.564706)));
	definedcolors.push_back(DefinedColor("violetred1", Color(1.000000, 0.243137, 0.588235)));
	definedcolors.push_back(DefinedColor("violetred2", Color(0.933333, 0.227451, 0.549020)));
	definedcolors.push_back(DefinedColor("violetred3", Color(0.803922, 0.196078, 0.470588)));
	definedcolors.push_back(DefinedColor("violetred4", Color(0.545098, 0.133333, 0.321569)));
	definedcolors.push_back(DefinedColor("warmgrey", Color(0.501961, 0.501961, 0.411765)));
	definedcolors.push_back(DefinedColor("wheat", Color(0.960784, 0.870588, 0.701961)));
	definedcolors.push_back(DefinedColor("wheat1", Color(1.000000, 0.905882, 0.729412)));
	definedcolors.push_back(DefinedColor("wheat2", Color(0.933333, 0.847059, 0.682353)));
	definedcolors.push_back(DefinedColor("wheat3", Color(0.803922, 0.729412, 0.588235)));
	definedcolors.push_back(DefinedColor("wheat4", Color(0.545098, 0.494118, 0.400000)));
	definedcolors.push_back(DefinedColor("white", Color(1.000000, 1.000000, 1.000000)));
	definedcolors.push_back(DefinedColor("whitesmoke", Color(0.960784, 0.960784, 0.960784)));
	definedcolors.push_back(DefinedColor("yellow1", Color(1.000000, 1.000000, 0.000000)));
	definedcolors.push_back(DefinedColor("yellow2", Color(0.933333, 0.933333, 0.000000)));
	definedcolors.push_back(DefinedColor("yellow3", Color(0.803922, 0.803922, 0.000000)));
	definedcolors.push_back(DefinedColor("yellow4", Color(0.545098, 0.545098, 0.000000)));
}