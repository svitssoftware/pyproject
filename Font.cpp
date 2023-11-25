#include "Font.h"
#include <iostream>
using namespace std;

void Font::LoadFromFile(string path, int xwin, int ywin)
{
    if (FT_Init_FreeType(&this->library))
    {
        cout << "Couldn't init FreeType library." << endl;
    }
    this->factor = (float)xwin / (float)ywin;

    this->font = new FontArray();
    this->Load(this->font, 15, path, xwin, ywin);
    this->sfont = new FontArray();
    this->Load(this->sfont, 30, path, xwin, ywin);
}

void Font::Load(FontArray*& fa, int size, string path, int xwin, int ywin)
{
    FT_New_Face(this->library, path.c_str(), 0, &this->face);

    FT_Set_Pixel_Sizes(this->face, 0, size);

    fa->glcharsize = ToGLSize(xwin, size);
    for (int c = 0; c < 840; c++)
    {
        //std::wcout << wchar_t(c) << endl;
        if (FT_Load_Char(this->face, wchar_t(c), FT_LOAD_RENDER))
        {
            wcout << L"Couldn't load glyph " << wchar_t(c) << endl;
        }

        Char ch;
        ch.sizex = ToGLSize(xwin, this->face->glyph->bitmap.width) * this->factor;
        ch.sizey = ToGLSize(ywin, this->face->glyph->bitmap.rows);
        ch.advance = ToGLSize(xwin, this->face->glyph->advance.x / 64) * this->factor;
        ch.xbearing = ToGLSize(xwin, this->face->glyph->bitmap_left) * this->factor;
        ch.ybearing = ToGLSize(ywin, this->face->glyph->bitmap_top);

        glGenTextures(1, &ch.texName);

        glBindTexture(GL_TEXTURE_2D, ch.texName);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, this->face->glyph->bitmap.width, this->face->glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, this->face->glyph->bitmap.buffer);

        glBindTexture(GL_TEXTURE_2D, 0);

        fa->chars.push_back(ch);
    }

    Char* ch = &fa->chars[(int)' '];
    ch->advance = fa->glcharsize / 4 * this->factor;

    ch = &fa->chars[9];
    ch->advance = fa->glcharsize * this->factor;
}
