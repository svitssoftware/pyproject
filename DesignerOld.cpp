#include "Designer.h"

void Designer::Designer::Set(float dx, float dy, float w, float h, Font* font, Color color)
{
    this->zx = dx;
    this->zy = dy;

    this->dwin.title.setFont(font);
    this->dwin.title.setColor(Color(1.0, 1.0, 1.0));
}

void Designer::Designer::ConEditor(TextEditor* ed)
{
    this->con_editor = ed;
}

void Designer::Designer::SetSPL(vector<vector<int>>* ospl, vector<wstring>* nspl)
{
    this->obj_spec_prop_lists = ospl;
}

void Designer::Designer::SetWinSize(float w, float h, float gw)
{
    this->ww = w;
    this->wh = h;
    this->gww = gw;
}

void Designer::Designer::Render()
{
    if (this->visible)
    {
        this->dwin.back.Render();
        this->dwin.titleback.Render();
        this->dwin.title.Render();
        for(auto & b : this->dbuttons)
            b.button.Render();
    }
}

void Designer::Designer::dMouseDown(POINT md)
{
    
}

void Designer::Designer::ShapeAfterFile(wifstream& input)
{
    wstring line;
    bool winfound = false;
    wstring winname;
    DParserTree tree;
    int lineindex = 0;
    bool admissable;

    while (getline(input, line))
    {
        dParserLine dline;
        admissable = false;
        wstring token;
        bool texting = false;
        for (int i = 0; i < line.size(); i++)
        {
            switch (line[i])
            {
            case ' ':
                if (!texting)
                {
                    if (token.size() > 0)
                    {
                        dline.tokens.push_back(token);
                        token.clear();
                    }
                }
                else
                {
                    token.push_back(line[i]);
                }
                break;
            case '=':
            case '.':
            case ',':
                if (!texting)
                {
                    admissable = true;
                    if (token.size() > 0)
                    {
                        dline.tokens.push_back(token);
                        token.clear();
                    }

                    token.push_back(line[i]);
                    dline.tokens.push_back(token);
                    token.clear();
                }
                else
                {
                    token.push_back(line[i]);
                }
                break;
            case '(':
            case ')':
            case ':':
                if (!texting)
                {
                    if (token.size() > 0)
                    {
                        dline.tokens.push_back(token);
                        token.clear();
                    }
                }
                else
                {
                    token.push_back(line[i]);
                }
                break;
            case '\"':
                texting = !texting;
                break;
            default:
                token.push_back(line[i]);
                break;
            }
        }
        if (admissable)
        {
            tree.lines.push_back(dline);
        }
        lineindex = lineindex + 1;
    }

    for (auto& each : tree.lines)
    {
        DesignerObject* obj = nullptr;
        //If first token(object) is already defined
        for (int i = 0; i < this->objects.size(); i++)
        {
            if (this->objects[i].name == each.tokens[0])
            {
                obj = &objects[i];
                goto SetProperty;
            }
        }

        if (each.tokens[1] == L"=")
        {
            //Set new object
            DesignerObject objs;
            obj = &objs;

            //Set object name
            obj->name = each.tokens[0];

            //Set object type
            if (each.tokens[2] == L"Tk")
            {
                obj->type = d_Window;
                obj->index = 0;
            }
            else if (each.tokens[2] == L"Button")
            {
                obj->type = d_Button;
                dButton but;
                this->dbuttons.push_back(but);
                obj->index = this->dbuttons.size() - 1;
                this->dbuttons[obj->index].button.text.setFont(this->dwin.title.font);
                this->dbuttons[obj->index].button.back.qolor = Color(0.94117647059, 0.94117647059, 0.94117647059);
            }
            else if (each.tokens[2] == L"Entry")
            {
                obj->type = d_Entry;
            }

            //obj->properties.domainindexs = &this->obj_spec_prop_lists->operator[](obj->type);

            if (each.tokens.size() > 3)
            {
                each.tokens.erase(each.tokens.begin(), each.tokens.begin() + 2);
                this->ProcessDefineSetObjectDefinition(each.tokens, obj);
            }

            this->objects.push_back(*obj);
        }
        goto End;

    SetProperty:     
        switch (obj->type)
        {
        case d_Button:
            if (each.tokens[2] == L"pack")
            {
                float b_size = this->dwin.title.font->font->glcharsize * (this->dbuttons[obj->index].button.text.data.size() + 1);
                this->dbuttons[obj->index].button.Set(
                    (this->dwin.back.x2 - this->dwin.back.x1 - b_size)/2 + this->dwin.back.x1,
                    this->zy - ToGLSize(wh, 30) - 0.01,
                    b_size, 
                    ToGLSize(wh, 22),
                    this->dbuttons[obj->index].button.back.qolor,
                    this->dwin.title.font,
                    Center);
                this->dbuttons[obj->index].button.SetOutline(Color(0, 0, 0));
            }
            break;
        case d_Window:
            if (each.tokens[2] == L"title")
            {
                this->dwin.title.data = each.tokens[3];
            }
            else if (each.tokens[2] == L"geometry")
            {
                int br = each.tokens[3].find_first_of('x', 0);
                if (br != -1)
                {
                    int x = stoi(each.tokens[3].substr(0, br));
                    int y = stoi(each.tokens[3].substr(br + 1, each.tokens[3].size() - 1 - br));
                    this->dwin.back.Set(this->zx + 0.01, this->zy - 0.01 - ToGLSize(wh, 30), ToGLSize(ww, x)* (gww / 2), ToGLSize(wh, y),
                        Color(0.941, 0.941, 0.941));
                    this->dwin.titleback.Set(this->zx + 0.01, this->zy - 0.01, this->dwin.back.x2 - this->dwin.back.x1, ToGLSize(wh, 30), 
                        Color(0.0f, 0.24f, 0.89f));
                    this->dwin.title.setPosition(this->zx + 0.03, this->zy - 0.035);
                }
            }
            break;
        }

    End:
        0;
    }
}

void Designer::Designer::ProcessDefineSetObjectDefinition(vector<wstring>& tokens, DesignerObject* object)
{
    int phase = 0;
    for (int i = 0; i < tokens.size(); i++)
    {
        if (phase == 1)
        {
            if (tokens[i - 2] == L"text")
            {
                switch (object->type)
                {
                case d_Button:
                {
                    this->dbuttons[object->index].button.text.data = tokens[i];
                }
                }
            }
            else if (tokens[i - 2] == L"bg")
            {
                switch (object->type)
                {
                case d_Button:
                {
                    Color c = string_to_color(string(tokens[i].begin(), tokens[i].end()), *this->definedcolors);
                    this->dbuttons[object->index].button.back.qolor = c;
                }
                }
            }
            else if (tokens[i - 2] == L"fg")
            {
                switch (object->type)
                {
                case d_Button:
                {
                    Color c = string_to_color(string(tokens[i].begin(), tokens[i].end()), *this->definedcolors);
                    this->dbuttons[object->index].button.text.setColor(c);
                }
                }
            }
        }
        if (tokens[i] == L"=")
            phase = 1;
        else if (tokens[i] == L",")
            phase = 0;
    }
}

void Designer::Designer::DesignerLineChanged(vector<int> clines)
{
    for (int i = 0; i < clines.size(); i++)
    {
        //this->ProcessLine(this->con_editor->lines[clines[i]]);
    }
    return;
}

void Designer::Designer::ProcessLine(TLine& line)
{
    bool winfound;
    wstring winname;
    if (this->dwin.title.data != L"")
    {
        winfound = true;
        winname = this->dwin.title.data;;
    }
    else
    {
        winfound = false;
    }
    bool admissable;

    dParserLine dline;
    admissable = false;
    wstring token;
    bool texting = false;

    for (int i = 0; i < line.tokens.size(); i++)
    {
        for (int u = 0; u < line.tokens[i].token.size(); u++)
        {
            switch (line.tokens[i].token[u])
            {
            case ' ':
                if (!texting)
                {
                    if (token.size() > 0)
                    {
                        dline.tokens.push_back(token);
                        token.clear();
                    }
                }
                else
                {
                    token.push_back(line.tokens[i].token[u]);
                }
                break;
            case '=':
            case '.':
            case ',':
                if (!texting)
                {
                    admissable = true;
                    if (token.size() > 0)
                    {
                        dline.tokens.push_back(token);
                        token.clear();
                    }

                    token.push_back(line.tokens[i].token[u]);
                    dline.tokens.push_back(token);
                    token.clear();
                }
                else
                {
                    token.push_back(line.tokens[i].token[u]);
                }
                break;
            case '(':
            case ')':
            case ':':
                if (!texting)
                {
                    if (token.size() > 0)
                    {
                        dline.tokens.push_back(token);
                        token.clear();
                    }
                }
                else
                {
                    token.push_back(line.tokens[i].token[u]);
                }
                break;
            case '\"':
                texting = !texting;
                break;
            default:
                token.push_back(line.tokens[i].token[u]);
                break;
            }
        }
    }
    if (admissable)
    {
        goto WasAdmissable;
    }
    return;
WasAdmissable:
    DesignerObject* obj = nullptr;
    //If first token(object) is already defined
    for (int i = 0; i < this->objects.size(); i++)
    {
        if (this->objects[i].name == dline.tokens[0])
        {
            obj = &objects[i];
            goto SetProperty;
        }
    }

    if (dline.tokens[1] == L"=")
    {
        //Set new object
        DesignerObject objs;
        obj = &objs;

        //Set object name
        obj->name = dline.tokens[0];

        //Set object type
        if (dline.tokens[2] == L"Tk")
        {
            obj->type = d_Window;
            obj->index = 0;
        }
        else if (dline.tokens[2] == L"Button")
        {
            obj->type = d_Button;
            dButton but;
            this->dbuttons.push_back(but);
            obj->index = this->dbuttons.size() - 1;
            this->dbuttons[obj->index].button.text.setFont(this->dwin.title.font);
            this->dbuttons[obj->index].button.back.qolor = Color(0.94117647059, 0.94117647059, 0.94117647059);
        }
        else if (dline.tokens[2] == L"Entry")
        {
            obj->type = d_Entry;
        }

        //obj->properties.domainindexs = &this->obj_spec_prop_lists->operator[](obj->type);

        if (dline.tokens.size() > 3)
        {
            dline.tokens.erase(dline.tokens.begin(), dline.tokens.begin() + 2);
            this->ProcessDefineSetObjectDefinition(dline.tokens, obj);
        }

        this->objects.push_back(*obj);
    }
    goto End;

SetProperty:
    switch (obj->type)
    {
    case d_Button:
        if (dline.tokens[2] == L"pack")
        {
            float b_size = this->dwin.title.font->font->glcharsize * (this->dbuttons[obj->index].button.text.data.size() + 1);
            this->dbuttons[obj->index].button.Set(
                (this->dwin.back.x2 - this->dwin.back.x1 - b_size) / 2 + this->dwin.back.x1,
                this->zy - ToGLSize(wh, 30) - 0.01,
                b_size,
                ToGLSize(wh, 22),
                this->dbuttons[obj->index].button.back.qolor,
                this->dwin.title.font,
                Center);
            this->dbuttons[obj->index].button.SetOutline(Color(0, 0, 0));
        }
        break;
    case d_Window:
        if (dline.tokens[2] == L"title")
        {
            this->dwin.title.data = dline.tokens[3];
        }
        else if (dline.tokens[2] == L"geometry")
        {
            int br = dline.tokens[3].find_first_of('x', 0);
            if (br != -1)
            {
                int x = stoi(dline.tokens[3].substr(0, br));
                int y = stoi(dline.tokens[3].substr(br + 1, dline.tokens[3].size() - 1 - br));
                this->dwin.back.Set(this->zx + 0.01, this->zy - 0.01 - ToGLSize(wh, 30), ToGLSize(ww, x) * (gww / 2), ToGLSize(wh, y),
                    Color(0.941, 0.941, 0.941));
                this->dwin.titleback.Set(this->zx + 0.01, this->zy - 0.01, this->dwin.back.x2 - this->dwin.back.x1, ToGLSize(wh, 30),
                    Color(0.0f, 0.24f, 0.89f));
                this->dwin.title.setPosition(this->zx + 0.03, this->zy - 0.035);
            }
        }
        break;
    }
End:
    0;
}