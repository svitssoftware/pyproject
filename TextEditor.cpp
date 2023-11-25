#include "TextEditor.h"

#pragma region Graphics
TextEditor::TextEditor()
{

}

void TextEditor::Set(float x, float y, float width, float height, Color color, Font* vfont)
{
	this->yborder = y - 0.04;
	this->font = vfont;
	this->w_border.Set(x, y, width, height, Color(0.2, 0.2, 0.2));
	this->border.Set(x + 0.04, y - 0.04, width - 0.08, height - 0.08, color);
	this->Resize(width, height);
	this->Cursor.Set(this->border.x1 + 0.1, this->yborder - 0.01, this->border.x1 + 0.1, this->yborder - this->font->font->glcharsize*1.7 - 0.01, Color(0.9, 0.9, 0.9));
	this->suggestionbox.back.Set(-1, -1, 0.5, 8 * (this->font->font->glcharsize + 0.02) + 0.2, color);
	this->suggestionbox.out.Set(-1, -1, 0.5, this->suggestionbox.back.x2 - this->suggestionbox.back.x1, Color(0.50196078431, 0.51764705882, 0.52941176471));
	this->suggestionbox.data.s_font = this->font;
	this->suggestionbox.data.s_width = 0.4;
	this->suggestionbox.data.s_height = this->suggestionbox.back.x2 - this->suggestionbox.back.x1;
	this->suggestionbox.data.x_position = -1;
	this->suggestionbox.data.y_position = -1;
	this->suggestionbox.visible = false;
}

void TextEditor::SetDesignerBelongine(int d)
{
	this->designernum = d;
}

void TextEditor::Resize(float x, float y)
{
	int m = (y - 0.005) / (this->font->font->glcharsize + 0.02);
 	this->maxlines = (int)m;
}

void TextEditor::Render()
{
	if (this->visible)
	{
		this->w_border.Render();
		this->border.Render();

		/*if (this->showselection)
		{
			for (auto& m : this->selection.markers)
				m.Render();
		}*/

		this->RenderLines();

		if (this->Cursor.q1 < this->yborder && this->Cursor.q2 > this->border.y2)
	    	this->Cursor.Render();

		this->suggestionbox.Render();
	}
}

void TextEditor::RenderLines()
{
	for (int i = this->startline; i < this->Lines.size() && i < this->maxlines + this->startline; i++)
	{
		Text linenum;
		linenum.setFont(this->font);
		linenum.setPosition(this->border.x1 + 0.02, this->yborder - (i  - this->startline + 1) * (this->font->font->glcharsize + 0.02) + 0.016);
		linenum.setData(to_wstring(i + 1));
		linenum.setColor(Color(0.16862745098, 0.56862745098, 0.68627450980));
		linenum.Render();

		float xpos = this->border.x1 + 0.1, rowpos = this->yborder - (i + 1 - this->startline)*(this->font->font->glcharsize + 0.02);

		for (int n = 0; n < this->Lines[i].tokens.size(); n++) 
		{
			glColor3f(this->smart->colorlabel[(int)this->Lines[i].tokens[n].type].r,
				this->smart->colorlabel[(int)this->Lines[i].tokens[n].type].g,
			this->smart->colorlabel[(int)this->Lines[i].tokens[n].type].b);

			for (int m = 0; m < this->Lines[i].tokens[n].token.size(); m++)
			{
				wchar_t wch = this->Lines[i].tokens[n].token[m];

				switch (wch)
				{
				case L' ':
				{
					xpos = xpos + this->font->font->glcharsize*0.95;
					break;
				}
				case 9:
				{
					xpos = xpos + this->font->font->glcharsize*1.9;
					break;
				}
				default:
				{
					Char* ch = &this->font->font->chars[(int)wch];

					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
					glBindTexture(GL_TEXTURE_2D, ch->texName);

					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos + ch->xbearing, rowpos + ch->ybearing);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + ch->xbearing + ch->sizex, rowpos + ch->ybearing);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + ch->xbearing + ch->sizex, rowpos + ch->ybearing - ch->sizey);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos + ch->xbearing, rowpos + ch->ybearing - ch->sizey);
					glEnd();

					glBindTexture(GL_TEXTURE_2D, 0);

					xpos = xpos + this->font->font->glcharsize*0.95;

					break;
				}
				}
			}
		}
	}
}

void TextEditor::Add(wchar_t key, bool loading)
{
	if (this->Lines.size() == 0)
	{
		TLine line;
		this->Lines.push_back(line);
		this->Current_Line = 0;
		TToken token;
		token.type = Blank;
		this->Lines[this->Current_Line].tokens.push_back(token);
		this->Token_Iterator = 0;
		this->Letter_Iterator = 0;
	}

	switch (key)
	{
	case 9:
	case ' ':
	{
		if (this->Letter_Iterator == 0)
		{
			if (this->Token_Iterator > 0 && this->Lines[this->Current_Line].tokens[this->Token_Iterator-1].type == Blank)
			{
				this->Lines[this->Current_Line].tokens[this->Token_Iterator-1].token.push_back(key);
			}
			else if(this->Token_Iterator > this->Lines[this->Current_Line].tokens.size() - 1 && this->Lines[this->Current_Line].tokens[this->Token_Iterator + 1].type == Blank)
			{
				this->Lines[this->Current_Line].tokens[this->Token_Iterator - 1].token.insert(this->Lines[this->Current_Line].tokens[this->Token_Iterator - 1].token.begin(), key);
			}
			else
			{
				TToken token;
				token.token = key;
				token.type = Blank;
				this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator, token);
				this->Token_Iterator++;
			}
		}
		else if (this->Lines[this->Current_Line].tokens[this->Token_Iterator].type == Characters)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.insert(this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.begin() + this->Letter_Iterator, key);
			this->Letter_Iterator = this->Letter_Iterator + 1;
		}
		else
		{	
			if (this->Letter_Iterator == this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size())
			{
				this->Tokenize();
				TToken token;
				token.token = key;
				token.type = Blank;
				this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, token);

				TToken secondtoken;
				secondtoken.token = L"";
				secondtoken.type = Plain;
				this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 2, secondtoken);

				this->Token_Iterator = this->Token_Iterator + 2;
				this->Letter_Iterator = 0;
			}
			else
			{
				TToken secondtoken;
				secondtoken.token = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.substr(this->Letter_Iterator, this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size() - this->Letter_Iterator);
				secondtoken.type = Plain;
				this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, secondtoken);

				this->Lines[this->Current_Line].tokens[this->Token_Iterator].token = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.substr(0, this->Letter_Iterator);
				this->Lines[this->Current_Line].tokens[this->Token_Iterator].type = Plain;

				TToken token;
				token.token = key;
				token.type = Blank;
				this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, token);
				this->Token_Iterator = this->Token_Iterator + 1;
				this->Letter_Iterator = 0;
			}
		}
		break;
	}
	case '\"':
	{
		if (this->Lines[this->Current_Line].tokens[this->Token_Iterator].type == Characters)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.insert(this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.begin() + this->Letter_Iterator, key);
			TToken token;
			token.type = Plain;
			this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, token);
			this->Token_Iterator = this->Token_Iterator + 1;
			this->Letter_Iterator = 0;
		}
		else
		{
			TToken token;
			token.token = key;
			token.type = Characters;
			this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, token);
			this->Token_Iterator = this->Token_Iterator + 1;
			this->Letter_Iterator = 1;
		}
		break;
	}
	case '=':
	{
		if (this->Lines[this->Current_Line].tokens[this->Token_Iterator].type == Characters)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.insert(this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.begin() + this->Letter_Iterator, key);
			this->Letter_Iterator = this->Letter_Iterator + 1;
		}
		else
		{
			if (!this->in_brackets && this->Lines[this->Current_Line].tokens.size() > 0)
			{
				for (int i = this->Token_Iterator; i > -1; i--)
				{
					if (this->Lines[this->Current_Line].tokens[i].type == Plain && this->Lines[this->Current_Line].tokens[i].token.size() > 0)
					{
						this->variables.push_back(this->Lines[this->Current_Line].tokens[i].token);
						this->Lines[this->Current_Line].tokens[i].type = Object;
						break;
					}
				}
				
			}
			
		Part2:
			if (this->Lines[this->Current_Line].tokens[Token_Iterator].token.size() == 0)
			{
				this->Lines[this->Current_Line].tokens[Token_Iterator].token.push_back(key);
				this->Lines[this->Current_Line].tokens[Token_Iterator].type = Symbol;
			}
			else
			{
				TToken token;
				token.token = key;
				token.type = Symbol;
				this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, token);
				this->Token_Iterator = this->Token_Iterator + 1;
			}
			TToken nexttoken;
			nexttoken.type = Plain;
			this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, nexttoken);
			this->Token_Iterator = this->Token_Iterator + 1;
			this->Letter_Iterator = 0;
		}
		break;
	}
	case '(':
		if (this->Lines[this->Current_Line].tokens[this->Token_Iterator].type == Characters)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.insert(this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.begin() + this->Letter_Iterator, key);
			this->Letter_Iterator = this->Letter_Iterator + 1;
		}
		else
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].type = Function;
			TToken token;
			token.token = key;
			token.type = Symbol;
			this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, token);
			this->Token_Iterator = this->Token_Iterator + 1;
			TToken nexttoken;
			nexttoken.type = Plain;
			this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, nexttoken);
			this->Token_Iterator = this->Token_Iterator + 1;
			this->Letter_Iterator = 0;
		}
		this->in_brackets = true;
		break;
	case ')':
		if (this->Lines[this->Current_Line].tokens[this->Token_Iterator].type == Characters)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.insert(this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.begin() + this->Letter_Iterator, key);
			this->Letter_Iterator = this->Letter_Iterator + 1;
		}
		else
		{
			this->Tokenize();
			if (this->Lines[this->Current_Line].tokens[Token_Iterator].token.size() == 0)
			{
				this->Lines[this->Current_Line].tokens[Token_Iterator].token.push_back(key);
				this->Lines[this->Current_Line].tokens[Token_Iterator].type = Symbol;
			}
			else
			{
				TToken token;
				token.token = key;
				token.type = Symbol;
				this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, token);
				this->Token_Iterator = this->Token_Iterator + 1;
			}
			TToken nexttoken;
			nexttoken.type = Plain;
			this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, nexttoken);
			this->Token_Iterator = this->Token_Iterator + 1;
			this->Letter_Iterator = 0;
		}
		this->in_brackets = false;
		break;
	case '/':
	case '.':
	case '?':
	case '*':
	case '&':	
	case '#':
	case '!':
	case ',':
	case ':':
	case '-':
	case '<':
	case '>':
	case '+':
	{
		if (this->Lines[this->Current_Line].tokens[this->Token_Iterator].type == Characters)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.insert(this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.begin() + this->Letter_Iterator, key);
			this->Letter_Iterator++;
		}
		else
		{
			if (this->Lines[this->Current_Line].tokens[this->Token_Iterator].type != Symbol)
			    this->Tokenize();
			if (this->Lines[this->Current_Line].tokens[Token_Iterator].token.size() == 0)
			{
				this->Lines[this->Current_Line].tokens[Token_Iterator].token.push_back(key);
				this->Lines[this->Current_Line].tokens[Token_Iterator].type = Symbol;
			}
			else
			{
				TToken token;
				token.token = key;
				token.type = Symbol;
				this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, token);
				this->Token_Iterator++;
			}
			TToken nexttoken;
			nexttoken.type = Plain;
			this->Lines[this->Current_Line].tokens.insert(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator + 1, nexttoken);
			this->Token_Iterator++;
			this->Letter_Iterator = 0;
		}
		break;
	}
	case 8203:
		break;
	case '\n':
		this->Enter();
		this->Current_Line_Position = 0;
		return;
	default:
	{
		this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.insert(this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.begin() + this->Letter_Iterator, key);
		this->Letter_Iterator++;
		break;
	}
	}

	this->Cursor.p1 = this->Cursor.p1 + this->font->font->chars[(int)key].advance;
	this->Cursor.p2 = this->Cursor.p2 + this->font->font->chars[(int)key].advance;
	this->Current_Line_Position++;
	this->Lines[this->Current_Line].size++;
}

void TextEditor::Reset()
{
	//ToDo
}

#include <iostream>
void TextEditor::Enter() // DONE
{
	if (importing)
	{
		this->imports.push_back(this->Lines[this->Current_Line].tokens[this->Token_Iterator].token);
		importing = false;
	}
	this->PushNewLine(this->Current_Line + 1);
	if (this->Current_Line > 0 && this->Lines[this->Current_Line - 1].tokens.size() > this->Token_Iterator)
	{
		if (this->Lines[this->Current_Line - 1].tokens[this->Token_Iterator].token.size() > this->Letter_Iterator)
		{
			this->Lines[this->Current_Line].tokens.push_back(
				TToken(this->Lines[this->Current_Line - 1].tokens[this->Token_Iterator].token.substr(this->Letter_Iterator, this->Lines[this->Current_Line - 1].tokens[this->Token_Iterator].token.size() - this->Letter_Iterator)));
			this->Lines[this->Current_Line - 1].tokens[this->Token_Iterator].token.erase(this->Lines[this->Current_Line - 1].tokens[this->Token_Iterator].token.begin() + this->Letter_Iterator, this->Lines[this->Current_Line - 1].tokens[this->Token_Iterator].token.end());
		}
		for (int i = this->Token_Iterator + 1; i < this->Lines[this->Current_Line - 1].tokens.size(); i++)
		{
			this->Lines[this->Current_Line].tokens.push_back(this->Lines[this->Current_Line - 1].tokens[i]);
		}
		this->Lines[this->Current_Line - 1].tokens.erase(this->Lines[this->Current_Line - 1].tokens.begin() + this->Token_Iterator + 1, this->Lines[this->Current_Line - 1].tokens.end());
	}
	this->Cursor.q1 = this->yborder - (this->Current_Line) * (this->font->font->glcharsize + 0.02) - 0.013;
	this->Cursor.q2 = this->yborder - (this->Current_Line + 1) * (this->font->font->glcharsize + 0.02) - 0.013;
	this->Cursor.p1 = this->border.x1 + 0.1;
	this->Cursor.p2 = this->border.x1 + 0.1;
	this->Current_Line_Position = 0;
	this->Token_Iterator = 0;
	this->Letter_Iterator = 0;
}

#include <iostream>
void TextEditor::Backspace() //Done
{
	if (this->Lines.size() > 0)
	{		
		if (this->Letter_Iterator > 0)
		{
			//Delete one last character and move Cursor one character position back
			this->Letter_Iterator = this->Letter_Iterator - 1;
			Char* ch = &this->font->font->chars[(int)this->Lines[this->Current_Line].tokens[this->Token_Iterator].token[this->Letter_Iterator]];
			this->Cursor.p1 = this->Cursor.p1 - ch->advance;
			this->Cursor.p2 = this->Cursor.p2 - ch->advance;
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.erase(this->Letter_Iterator, 1);
		}
		else if (this->Token_Iterator > 0)
		{
			this->Token_Iterator = this->Token_Iterator - 1;
			this->Letter_Iterator = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size() - 1;
			if (this->Letter_Iterator == -1)
			{
				this->Lines[this->Current_Line].tokens.erase(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator);
				this->Token_Iterator = this->Token_Iterator - 1;
				this->Letter_Iterator = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size();
				this->Backspace();
				goto EndPart;
			}
			Char* ch = &this->font->font->chars[(int)this->Lines[this->Current_Line].tokens[this->Token_Iterator].token[this->Letter_Iterator]];
			this->Cursor.p1 = this->Cursor.p1 - ch->advance;
			this->Cursor.p2 = this->Cursor.p2 - ch->advance;
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.erase(this->Letter_Iterator, 1);

		}
		else if (this->Lines.size() > 1 && this->Current_Line > 0)
		{
			//When Cursor is on start of line, moves current line back to one upper line as backspace presses
			this->Current_Line = this->Current_Line - 1;
			if (this->Lines[this->Current_Line].tokens.size() > 0)
			{
				this->Token_Iterator = this->Lines[this->Current_Line].tokens.size() - 1;
				this->Letter_Iterator = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size();
			}
			else
			{
				TToken tt;
				tt.token = L"";
				this->Lines[this->Current_Line].tokens.push_back(tt);
				this->Token_Iterator = 0;
				this->Letter_Iterator = 0;
			}
			this->CursorOnEnd();
			for (auto& k : this->Lines[this->Current_Line + 1].tokens)
			{
				this->Lines[this->Current_Line].tokens.push_back(k);
			}
			this->Lines.erase(this->Lines.begin() + this->Current_Line + 1);			
		}	

		if (this->Lines[this->Current_Line].tokens.size() > 1)
		{
			if (this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size() == 0)
			{
				this->Lines[this->Current_Line].tokens.erase(this->Lines[this->Current_Line].tokens.begin() + this->Token_Iterator);
				if (this->Token_Iterator > 0)
				{
					this->Token_Iterator = this->Token_Iterator - 1;
					this->Letter_Iterator = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size();
				}
			}
		}
	EndPart:
		this->LineChanged(this->Current_Line);
	}
}

void TextEditor::Delete() //DONE
{
	if (this->Letter_Iterator < this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size())
	{
		this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.erase(this->Letter_Iterator, 1);
	}
	else if (this->Token_Iterator < this->Lines[this->Current_Line].tokens.size() - 1)
	{
		this->Token_Iterator = this->Token_Iterator + 1;
		this->Letter_Iterator = 0;
		this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.erase(this->Letter_Iterator, 1);
	}
	else if (this->Lines.size() - 1 > this->Current_Line)
	{
		for (auto& d : this->Lines[this->Current_Line + 1].tokens)
		{
			for(auto&c : d.token)
			this->Lines[this->Current_Line].tokens[Token_Iterator].token.push_back(c);
		}
		this->Lines.erase(this->Lines.begin() + this->Current_Line + 1);
	}
}

void TextEditor::MoveLeft()
{
	if (this->Token_Iterator == 0 && this->Letter_Iterator == 0)
	{
		if (this->Current_Line != 0)
		{
			this->Current_Line--;
			this->Token_Iterator = this->Lines[this->Current_Line].tokens.size()-1;
			this->Letter_Iterator = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size() - 1;
			this->Current_Line_Position = this->Lines[this->Current_Line].size;
			this->Cursor.q1 = this->yborder - this->Current_Line * (this->font->font->glcharsize + 0.02) - 0.013;
			this->Cursor.q2 = this->yborder - (this->Current_Line + 1) * (this->font->font->glcharsize + 0.02) - 0.013;
		}
	}
	else if (this->Letter_Iterator == 0)
	{
		this->Token_Iterator--;
		this->Letter_Iterator = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size();
		this->Current_Line_Position--;
	}
	else
	{
		this->Letter_Iterator--;
		this->Current_Line_Position--;
	}
	this->Cursor.p1 = this->border.x1 + 0.1 + this->font->font->glcharsize * 0.95 * this->Current_Line_Position;
	this->Cursor.p2 = this->Cursor.p1;
}

void TextEditor::MoveRight() 
{
	
}

void TextEditor::MoveUp() //DONE!!!
{
	if (this->Current_Line > 0)
	{
		this->Cursor.q1 = this->yborder - (this->Current_Line - 1) * (this->font->font->glcharsize + 0.02) - 0.013;
		this->Cursor.q2 = this->yborder - (this->Current_Line) * (this->font->font->glcharsize + 0.02) - 0.013;
		int iterator = 0;
		this->Current_Line = this->Current_Line - 1;
		for (int i = 0; i < this->Lines[this->Current_Line].tokens.size(); i++)
		{
			iterator = iterator + this->Lines[this->Current_Line].tokens[i].token.size();
			if (iterator >= this->Current_Line_Position)
			{
				this->Letter_Iterator = iterator - this->Current_Line_Position;
				this->Token_Iterator = i;
				return;
			}
		}
		this->Token_Iterator = this->Lines[this->Current_Line].tokens.size() - 1;
		this->Letter_Iterator = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size();
		this->CursorOnEnd();
	}
}

void TextEditor::MoveDown() //DONE!!!
{
	if (this->Current_Line < this->Lines.size() - 1)
	{

		this->Cursor.q1 = this->yborder - (this->Current_Line+1) * (this->font->font->glcharsize + 0.02) - 0.013;
		this->Cursor.q2 = this->yborder - (this->Current_Line+2) * (this->font->font->glcharsize + 0.02) - 0.013;
		int iterator = 0;
		this->Current_Line = this->Current_Line + 1;
		for (int i = 0; i < this->Lines[this->Current_Line].tokens.size(); i++)
		{
			iterator = iterator + this->Lines[this->Current_Line].tokens[i].token.size();
			if (iterator >= this->Current_Line_Position)
			{
				this->Letter_Iterator = iterator - this->Current_Line_Position;
				this->Token_Iterator = i;
				return;
			}
		}
		this->Token_Iterator = this->Lines[this->Current_Line].tokens.size() - 1;
		this->Letter_Iterator = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size();
		this->CursorOnEnd();
	}
}

void TextEditor::MouseWheel(short delta) //DONE!!!
{
	delta = delta / 40;
	this->startline = this->startline - delta;
	if (this->startline < 0)
		this->startline = 0;
	else if (this->startline > this->Lines.size() - 2)
		this->startline = this->Lines.size() - 2;
	else
	{
		this->Cursor.q1 = this->Cursor.q1 - (this->font->font->glcharsize + 0.02)*delta - 0.01;
		this->Cursor.q2 = this->Cursor.q2 - (this->font->font->glcharsize + 0.02)*delta - 0.01;
	}
}

void TextEditor::MouseDown(float x, float y) //DONE!!!
{
	int CLine = this->startline + (this->yborder - y) / (this->font->font->glcharsize + 0.02);
	if (CLine >= this->startline && CLine <= this->Lines.size())
	{
		this->Current_Line = CLine;
		float SLine = (x - this->border.x1 - 0.1) / (this->font->font->glcharsize * 0.95);
		int PLine = 0;
		for (int i = 0; i < this->Lines[this->Current_Line].tokens.size(); i++)
		{
			for (int j = 0; j < this->Lines[this->Current_Line].tokens[i].token.size(); j++)
			{
				float dif = PLine - SLine;
				if (dif < 0.5 && dif >= -0.5)
				{	
					this->Token_Iterator = i;
					this->Letter_Iterator = j;
					goto SetCursorPos;
					
				}
				if (this->Lines[this->Current_Line].tokens[i].token[j] == 9)
					PLine++;
				PLine++;
			}
		}
		this->Token_Iterator = this->Lines[this->Current_Line].tokens.size()-1;
		this->Letter_Iterator = this->Lines[this->Current_Line].tokens[this->Token_Iterator].token.size();
	SetCursorPos:
		this->Current_Line_Position = PLine;
		this->Cursor.p1 = this->border.x1 + 0.1 + this->font->font->glcharsize * 0.95 * PLine;
		this->Cursor.p2 = this->Cursor.p1;
		this->Cursor.q1 = this->yborder - (this->Current_Line-this->startline) * (this->font->font->glcharsize + 0.02) - 0.013;
		this->Cursor.q2 = this->yborder - (this->Current_Line + 1 - this->startline) * (this->font->font->glcharsize + 0.02) - 0.013;
	}
}

void TextEditor::MouseUp(float x, float y)
{
	
}


void TextEditor::MouseMove(float x, float y)
{
	
}

void TextEditor::CursorOnEnd()
{
	float xpos = 0;
	for (auto& t : this->Lines[this->Current_Line].tokens)
	{
		for (auto& w : t.token)
		{
			xpos = xpos + this->font->font->chars[(int)w].advance;
		}
	}
	this->cx = this->border.x1 + 0.1 + xpos;
	this->Cursor.p1 = this->cx;
	this->Cursor.p2 = this->cx;
	this->Cursor.q1 = this->yborder - this->Current_Line * (this->font->font->glcharsize + 0.02) - 0.013;
	this->Cursor.q2 = this->yborder - (this->Current_Line + 1) * (this->font->font->glcharsize + 0.02) - 0.013;

}

void TextEditor::keyDown(int key)
{
	switch (key)
	{
	case VK_DELETE:
		this->Delete();
		break;
	case VK_LEFT:
		this->MoveLeft();
		break;
	case VK_RIGHT:
		this->MoveRight();
		break;
	case VK_UP:
		this->MoveUp();
		break;
	case VK_DOWN:
		this->MoveDown();
		break;
	}
}

void TextEditor::keyClick(wchar_t key)
{
	switch (key)
	{
	case 8:
		this->Backspace();
		break;
	case 13:
		this->Enter();
		break;
	case 22:
	{
		if (OpenClipboard(nullptr))
		{		
			HANDLE hData = GetClipboardData(CF_TEXT);
			if (hData != nullptr)
			{
				char* pszText = static_cast<char*>(GlobalLock(hData));
				cout << pszText << endl;
				if (pszText != nullptr)
				{
					std::string text(pszText);
					this->Paste(wstring(text.begin(), text.end()));
				}
			}
			GlobalUnlock(hData);
		}
		CloseClipboard();
		break;
	}
	default:
		this->Add(key, false);
		break;
	}
}

void TextEditor::MouseHover(float x, float y)
{

}

void TextEditor::DblClick(float x, float y)
{
	if (this->suggestionbox.visible == true && this->suggestionbox.back.x1 < x && this->suggestionbox.back.x2 > x && this->suggestionbox.back.y1 > y && this->suggestionbox.back.y2 < y)
	{
		int line = ((this->suggestionbox.back.y1 - 0.02) - y) / (this->font->font->glcharsize + 0.02);
		if (line < this->suggestionbox.suggestions.size())
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].token = this->suggestionbox.suggestions[line].data;
			this->Tokenize();
		}
	}
}

void TextEditor::Paste(wstring data)
{
	this->Add(data);
}

void TextEditor::Copy()
{

}

void TextEditor::Suggest()
{
	if (!this->suggestionbox.visible)
	{
		this->suggestionbox.visible = true;
		this->suggestionbox.back.x1 = this->Cursor.p1 - this->font->font->glcharsize;
		this->suggestionbox.back.x2 = this->suggestionbox.back.x1 + 0.5;
		this->suggestionbox.back.y1 = this->yborder - (this->Current_Line + 1) * (this->font->font->glcharsize + 0.02) - 0.013;
		this->suggestionbox.back.y2 = this->suggestionbox.back.y1 - 0.3;
		this->suggestionbox.out.x1  = this->suggestionbox.back.x1;
		this->suggestionbox.out.x2  = this->suggestionbox.back.x2;
		this->suggestionbox.out.y1  = this->suggestionbox.back.y1;
		this->suggestionbox.out.y2  = this->suggestionbox.back.y2;
		this->suggestionbox.data.x_position = this->suggestionbox.back.x1;
		this->suggestionbox.data.y_position = this->suggestionbox.back.y1;
	}

	this->suggestionbox.suggestions.clear();
	if (this->importing)
	{
		this->smart->SuggestModules(&this->suggestionbox.suggestions, this->Lines[this->Current_Line].tokens[this->Token_Iterator].token, &this->suggestionbox.data);
	}
	else
	{
		this->smart->Suggest(&this->suggestionbox.suggestions, this->Lines[this->Current_Line].tokens[this->Token_Iterator].token, &this->suggestionbox.data);
	}
}

void TextEditor::HideSuggestion()
{
	this->suggestionbox.visible = false;
	this->suggestionbox.suggestions.clear();
}

void TextEditor::PushNewLine(int index)
{
	TLine ln;
	if (index == -1)
	{
		this->Lines.push_back(ln);
		this->Current_Line = this->Lines.size()-1;
	}
	else if (index > -1)
	{
		this->Lines.insert(this->Lines.begin() + index, ln);
		this->Current_Line = index;
	}
	this->Lines[this->Current_Line].tokens.push_back(TToken());
}

void TextEditor::ChangePosition(Locator& locator)
{
	this->Current_Line = locator.line;
	this->Token_Iterator = locator.token;
	this->Letter_Iterator = locator.letter;
}

void TextEditor::Tokenize()
{
	for (auto& each : this->smart->label.labels)
	{
		if (wstring(each.word.begin(), each.word.end()) == this->Lines[this->Current_Line].tokens[this->Token_Iterator].token)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].type = each.type;
			this->HideSuggestion();
			return;
		}
	}
	for (auto& each : this->smart->label.py_modules)
	{
		if (each == this->Lines[this->Current_Line].tokens[this->Token_Iterator].token)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].type = Import;
			this->HideSuggestion();
			return;
		}
	}	
	//this->Lines[this->currentline].tokens[this->w_token].type = Plain;
	for (auto& each : this->imports)
	{
		if (each == this->Lines[this->Current_Line].tokens[this->Token_Iterator].token)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].type = Import;
			return;
		}
	}
	for (auto& each : this->variables)
	{
		if (each == this->Lines[this->Current_Line].tokens[this->Token_Iterator].token)
		{
			this->Lines[this->Current_Line].tokens[this->Token_Iterator].type = Object;
			return;
		}
	}
}
int TextEditor::CalcLineLenght(TLine& line)
{
	int a = 0;
	for (auto& t : line.tokens)
		a = a + t.token.size();
	return a;
}
void TextEditor::LineChanged(int c)
{

}
#pragma endregion

void SuggestionBox::Render()
{
	if (this->visible)
	{
		this->back.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->out.Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (int i = 0; i < 8; i++)
		{
			if (i < this->suggestions.size())
			    this->suggestions[i].Render();
		}
	}
}
