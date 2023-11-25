#include "Editor.h"
#include <iostream>
using namespace std;

Editor::Editor(HWND* hwnd)
{
    this->target = hwnd;

    int                   pf;
    HDC                   hDC;
    PIXELFORMATDESCRIPTOR pfd;

    hDC = GetDC(*target);

    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    pf = ChoosePixelFormat(hDC, &pfd);

    SetPixelFormat(hDC, pf, &pfd);

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(*target, hDC);
}

void Editor::Load(int w, int h, float glw)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    WIDTH   = (float)w;
    HEIGHT  = (float)h;
    glWIDTH = (float)glw;

    this->font.LoadFromFile("font.ttf", WIDTH, HEIGHT);
    smart.FormSuggestionLabel(L"Studio\\SmartCode\\Label.xml");
    smart.FormColorLabel(L"Studio\\SmartCode\\Color.xml");

    //this->debugger = new Debugger();

    this->selected = nullptr;

    this->LoadLayout();
    this->LoadSPL();

    DefineColors(this->definedcolors);

    this->typenames.push_back(L"Button");
    this->typenames.push_back(L"Entry");
    this->typenames.push_back(L"Label");
    this->typenames.push_back(L"Checkbutton");
    this->typenames.push_back(L"Menu");
    this->typenames.push_back(L"Combobox");
    this->typenames.push_back(L"Listbox");
    this->typenames.push_back(L"Menubutton");
    this->typenames.push_back(L"Canvas");
    this->typenames.push_back(L"Scale");
    this->typenames.push_back(L"Scrollbar");
    this->typenames.push_back(L"Toplevel");
    this->typenames.push_back(L"Frame");
    this->typenames.push_back(L"Radiobutton");
    this->typenames.push_back(L"Window");
}

void Editor::LoadProject(string path)
{
    this->project.LoadProject(path);
    for (auto& each : this->project.projfiles)
    {
        this->OpenFile(wstring(this->project.settings.projpath.begin(), this->project.settings.projpath.end()) + wstring(each.begin(), each.end()), -1, true);
    }
    this->exp.visible = true;
    this->exp.DispProj(&this->project);
}

void Editor::LoadSPL()
{
    wifstream ap = wifstream("Designer\\allproperties.xml\\");
    wstring line;
    while (getline(ap, line))
    {
        this->nspl.push_back(line);
    }
    ap.close();

    ap.open("Designer\\properties.xml\\");
    vector<int> objspecprops;
    while (getline(ap, line))
    {
        if (line == L"N")
        {
            this->ospl.push_back(objspecprops);
            objspecprops.clear();
        }
        else
        {
            //objspecprops.push_back(stoi(line));
        }
    }
}

void Editor::Render()
{
    if (!this->qrefreshing)
    {
        this->qtimer = this->qtimer + 1;
        if (this->qtimer == 3)
        {
            this->qtimer = 0;
        }
        else
        {
            return;
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->exp.Render();
    this->toolbox.Render();

    for (auto& each : this->l_editors)
        each->Render();

    for (auto& each : this->l_designers)
    {
        each.Render();
    }
    

    for (auto& each : this->tabs)
        each->button.Render();

    this->l_upb.Render();
    this->m_upb.Render();
    this->r_upb.Render();

    for (auto& each : this->l_panels)
        each->Render();

    for (auto& each : this->l_buttons)
        each.Render();

    this->h_button.Render();

    this->im.Render();

    switch (this->Procedure)
    {
    case 1:
        this->New_File_Chooser.Render();
        break;
    case 2:
        this->New_Project_Chooser.Render();
        break;
    }


    if (this->drag > -1)
    {
        switch (this->drag)
        {
        case 0:
            this->DragButton.Render();
            break;
        case 1:
            this->DragEntry.Render();
            break;
        case 2:
            this->DragLabel.Render();
            break;
        }
    }

    glFlush();
}

void Editor::keyClick(wchar_t key)
{
    switch (this->Procedure)
    {
        case 0:
            if (this->selected != nullptr)
            {
                switch (this->selected->type)
                {
                case Controls::ControlType::TextEditor:
                    this->l_editors[this->selected->index]->keyClick(key);
                    break;
                }
            }
            break;
        case 1:
            this->New_File_Chooser.KeyPressed(key);
            break;
        case 2:
            this->New_Project_Chooser.KeyPressed(key);

    }
    this->ProcessLineChanges();
}

void Editor::keyDown(int key)
{
    if (this->selected != nullptr)
    {
        switch (this->selected->type)
        {
        case Controls::ControlType::TextEditor:
            this->l_editors[this->selected->index]->keyDown(key);
            break;
        }
    }
    this->ProcessLineChanges();
}

void Editor::mouseDown(int x, int y)
{
    float gx = - 1.0f + ToGLSize(WIDTH,  (float)x);
    float gy = + 1.0f - ToGLSize(HEIGHT, (float)y);
    this->processcoord(gx);

    if (this->toolbox.visible && gx > this->toolbox.back.x1 && gx < this->toolbox.back.x2 && gy < this->toolbox.back.y1 && gy > this->toolbox.back.y2)
    {
        int i = this->toolbox.MouseDown(gx, gy);
        if (i > -1)
        {
            switch (i)
            {
            case 0:
            {
                this->DragButton.back.Set(gx, gy, 7 * this->font.font->glcharsize, ToGLSize(HEIGHT, 25), Color(0.7, 0.7, 0.7));
                this->DragButton.text.setFont(&this->font);
                this->DragButton.text.setPosition(gx + 0.01, gy - 0.016);
                this->DragButton.text.setColor(Color(0.2, 0.2, 0.2));
                this->DragButton.text.setData(L"Button");
                this->drag = 0;
                break;
            }
            case 1:
            {
                this->DragEntry.back.Set(gx, gy, 10 * this->font.font->glcharsize, ToGLSize(HEIGHT, 25), Color(1, 1, 1));
                this->DragEntry.out.Set(gx, gy, 10 * this->font.font->glcharsize, ToGLSize(HEIGHT, 25), Color(0, 0, 0));
                this->DragEntry.entry.setFont(&this->font);
                this->DragEntry.entry.setColor(Color(0.2, 0.2, 0.2));
                this->DragEntry.entry.setPosition(gx + 0.01, gy - 0.016);
                this->DragEntry.entry.setData(L"Write...");
                this->drag = 1;
                break;
            }
            case 2:
            {
                this->DragLabel.text.setFont(&this->font);
                this->DragLabel.text.setColor(Color(0.2f, 0.2f, 0.2f));
                this->DragLabel.text.setPosition(gx, gy);
                this->DragLabel.text.setData(L"New Label");
                this->drag = 2;
                break;
            }
            }
        }
    }

    this->selected = this->SearchControls(gx, gy);

    if (this->selected != nullptr)
    {
        switch (this->selected->type)
        {
        case Controls::ControlType::TextEditor:
            this->l_editors[this->selected->index]->MouseDown(gx, gy);
            break;
        case Controls::ControlType::Designer:
            this->l_designers[this->selected->index].MouseDown(gx, gy);
            break;
        }
    }
}

void Editor::mouseUp(int x, int y)
{
    float gx = - 1.0f + ToGLSize(WIDTH,  (float)x);
    float gy = + 1.0f - ToGLSize(HEIGHT, (float)y);
    this->processcoord(gx);


    switch (this->Procedure)
    {
    case 0:
    {
        if (this->drag != -1 && this->tabs[this->selectedtab]->holdtype == pDesignerFile)
        {
            if (this->l_designers[this->tabs[this->selectedtab]->index].MainWindow.back.x1 + this->l_designers[this->tabs[this->selectedtab]->index].transx < gx
                && this->l_designers[this->tabs[this->selectedtab]->index].MainWindow.back.x2 + this->l_designers[this->tabs[this->selectedtab]->index].transx > gx
                && this->l_designers[this->tabs[this->selectedtab]->index].MainWindow.back.y2 < gy
                && this->l_designers[this->tabs[this->selectedtab]->index].MainWindow.back.y1 > gy)
            {
                this->l_designers[this->tabs[this->selectedtab]->index].UserCreate(Designer::DesignerObjectType(this->drag), gx, gy);
            }
            this->drag = -1;
        }
        else if (this->exp.visible && gx > this->exp.back.x1 && gx < this->exp.back.x2 && gy < this->exp.back.y1 && gy > this->exp.back.y2)
        {
            int i = this->exp.PassClick(gx, gy);
            if (i > -1)
            {
                switch (this->tabs[i]->holdtype)
                {
                case ProjectFile:
                    for (auto& each : this->l_editors)
                        each->visible = false;
                    for (auto& each : this->l_designers)
                        each.visible = false;
                    for (auto& each : this->tabs)
                        each->button.back.qolor = Color(0.18f, 0.18f, 0.18f);
                    this->tabs[i]->button.back.qolor = Color(0.2f, 0.2f, 0.2f);
                    this->l_editors[this->tabs[i]->index]->visible = true;
                    this->selectedtab = i;
                    break;
                case fDesignerFile:
                case pDesignerFile:
                    for (auto& each : this->l_editors)
                        each->visible = false;
                    for (auto& each : this->l_designers)
                        each.visible = false;
                    for (auto& each : this->tabs)
                        each->button.back.qolor = Color(0.18f, 0.18f, 0.18f);
                    this->tabs[i]->button.back.qolor = Color(0.2f, 0.2f, 0.2f);
                    this->l_designers[this->tabs[i]->index].visible = true;
                    this->selectedtab = i;
                    break;
                }

                this->l_upb.p2 = this->tabs[this->selectedtab]->button.back.x1;
                this->m_upb.p1 = this->l_upb.p2;
                this->m_upb.p2 = this->tabs[this->selectedtab]->button.back.x2;
                this->r_upb.p1 = this->m_upb.p2;
            }
        }
        else
        {
            this->selected = this->SearchControls(gx, gy);

            if (this->selected != nullptr)
            {
                switch (this->selected->type)
                {
                case Controls::ControlType::Button:
                    this->h_button.visible = false;
                    this->ButtonClick(this->selected, this->count_temp);
                    break;
                case Controls::ControlType::TextEditor:
                    this->l_editors[this->selected->index]->MouseUp(gx, gy);
                    this->HideAllChilds();
                    break;
                case Controls::ControlType::Designer:
                    this->l_designers[this->selected->index].MouseUp(gx, gy);
                    this->HideAllChilds();
                    break;
                }
            }
            else
            {
                for (int i = 0; i < this->tabs.size(); i++)
                {
                    if (this->tabs[i]->button.back.x1 < gx && this->tabs[i]->button.back.x2 > gx && this->tabs[i]->button.back.y1 > gy && this->tabs[i]->button.back.y2 < gy)
                    {
                        switch (this->tabs[i]->holdtype)
                        {
                        case UnsavedNewFile:
                        case OpenedFile:
                        case ProjectFile:
                            for (auto& each : this->l_editors)
                                each->visible = false;
                            for (auto& each : this->l_designers)
                                each.visible = false;
                            for (auto& each : this->tabs)
                                each->button.back.qolor = Color(0.18f, 0.18f, 0.18f);
                            this->tabs[i]->button.back.qolor = Color(0.2f, 0.2f, 0.2f);
                            this->l_editors[this->tabs[i]->index]->visible = true;
                            this->selectedtab = i;
                            break;
                        case fDesignerFile:
                        case pDesignerFile:
                            for (auto& each : this->l_editors)
                                each->visible = false;
                            for (auto& each : this->l_designers)
                                each.visible = false;
                            for (auto& each : this->tabs)
                                each->button.back.qolor = Color(0.18f, 0.18f, 0.18f);
                            this->tabs[i]->button.back.qolor = Color(0.2f, 0.2f, 0.2f);
                            this->l_designers[this->tabs[i]->index].visible = true;
                            this->selectedtab = i;
                            break;
                        }

                        this->l_upb.p2 = this->tabs[this->selectedtab]->button.back.x1;
                        this->m_upb.p1 = this->l_upb.p2;
                        this->m_upb.p2 = this->tabs[this->selectedtab]->button.back.x2;
                        this->r_upb.p1 = this->m_upb.p2;
                    }
                }

                this->HideAllChilds();
            }
        }
    }
    case 1:
    {
        switch (this->New_File_Chooser.MouseClick(gx, gy))
        {
        case 1:
            //OK clicked
            this->CreateNewFile(this->New_File_Chooser.name.data.data);
            this->Procedure = 0;
            break;
        case 2:
            this->Procedure = 0;
            break;
        }
    }
    case 2:
        switch (this->New_Project_Chooser.MouseClick(gx, gy))
        {
        case 1:
            //OK clicked
            this->CreateNewProject(this->New_Project_Chooser.name.data.data, this->New_Project_Chooser.type.selected);
            this->Procedure = 0;
            break;
        case 2:
            this->Procedure = 0;
            break;
        }
    }
}

void Editor::mouseWheel(short delta)
{
    if (this->selected != nullptr)
    {
        switch (this->selected->type)
        {
        case Controls::ControlType::TextEditor:
            this->l_editors[this->selected->index]->MouseWheel(delta);
            break;
        }
    }
}

void Editor::mouseMove(int x, int y)
{
    float gx = -1.0f + ToGLSize(WIDTH, (float)x);
    float gy = +1.0f - ToGLSize(HEIGHT, (float)y);
    this->processcoord(gx);

    if (this->drag == -1)
    {
        Controls::Control* hovered = this->SearchControls(gx, gy);

        for (auto& each : this->l_buttons)
            each.back.qolor = Color(0.12f, 0.12f, 0.12f);

        if (hovered != nullptr)
        {
            switch (hovered->type)
            {
            case Controls::ControlType::Button:
                this->h_button.visible = true;
                this->l_buttons[hovered->index].back.qolor = Color(0.3f, 0.3f, 0.3f);
                this->h_button.Clear();
                this->h_button.AddVertice(Vertice(this->l_buttons[hovered->index].back.x1, this->l_buttons[hovered->index].back.y1));
                this->h_button.AddVertice(Vertice(this->l_buttons[hovered->index].back.x2, this->l_buttons[hovered->index].back.y1));
                this->h_button.AddVertice(Vertice(this->l_buttons[hovered->index].back.x2, this->l_buttons[hovered->index].back.y2));
                this->h_button.AddVertice(Vertice(this->l_buttons[hovered->index].back.x1, this->l_buttons[hovered->index].back.y2));
                break;
            case Controls::ControlType::TextEditor:
                this->l_editors[hovered->index]->MouseMove(gx, gy);
                break;
            }
        }
    }
    else
    {
        switch (drag)
        {
        case 0:
        {
            float width = this->DragButton.back.x2 - this->DragButton.back.x1;
            float height = this->DragButton.back.y1 - this->DragButton.back.y2;
            this->DragButton.back.x1 = gx;
            this->DragButton.back.x2 = this->DragButton.back.x1 + width;
            this->DragButton.back.y1 = gy;
            this->DragButton.back.y2 = this->DragButton.back.y1 - height;
            this->DragButton.text.setPosition(gx + 0.01, gy - 0.016);
            break;
        }
        case 1:
        {
            float width = this->DragEntry.back.x2 - this->DragEntry.back.x1;
            float height = this->DragEntry.back.y1 - this->DragEntry.back.y2;
            this->DragEntry.back.x1 = gx;
            this->DragEntry.back.x2 = this->DragEntry.back.x1 + width;
            this->DragEntry.back.y1 = gy;
            this->DragEntry.back.y2 = this->DragEntry.back.y1 - height;
            this->DragEntry.out.x1 = this->DragEntry.back.x1;
            this->DragEntry.out.x2 = this->DragEntry.back.x2;
            this->DragEntry.out.y1 = this->DragEntry.back.y1;
            this->DragEntry.out.y2 = this->DragEntry.back.y2;
            this->DragEntry.entry.setPosition(gx + 0.01, gy - 0.016);
            break;
        }
        case 2:
            this->DragLabel.text.x = gx;
            this->DragLabel.text.y = gy;
            break;
        }
    }
}

void Editor::mouseDblClick(int x, int y)
{
    float gx = -1.0f + ToGLSize(WIDTH, (float)x);
    float gy = +1.0f - ToGLSize(HEIGHT, (float)y);
    this->processcoord(gx);

    if (this->exp.visible && gx > this->exp.back.x1 && gx < this->exp.back.x2 && gy < this->exp.back.y1 && gy > this->exp.back.y2)
    {
        int i = this->exp.PassClick(gx, gy);
        if (i > -1)
        {
            switch (this->tabs[i]->holdtype)
            {
            case ProjectFile:
                for (auto& each : this->l_editors)
                    each->visible = false;
                for (auto& each : this->l_designers)
                    each.visible = false;
                for (auto& each : this->tabs)
                    each->button.back.qolor = Color(0.18f, 0.18f, 0.18f);
                this->tabs[i]->button.back.qolor = Color(0.2f, 0.2f, 0.2f);
                this->l_editors[this->tabs[i]->index]->visible = true;
                this->selectedtab = i;
                break;
            case fDesignerFile:
            case pDesignerFile:
                for (auto& each : this->l_editors)
                    each->visible = false;
                for (auto& each : this->l_designers)
                    each.visible = false;
                for (auto& each : this->tabs)
                    each->button.back.qolor = Color(0.18f, 0.18f, 0.18f);
                this->tabs[i]->button.back.qolor = Color(0.2f, 0.2f, 0.2f);
                this->l_designers[this->tabs[i]->index].visible = true;
                this->selectedtab = i;
                break;
            }

            this->l_upb.p2 = this->tabs[this->selectedtab]->button.back.x1;
            this->m_upb.p1 = this->l_upb.p2;
            this->m_upb.p2 = this->tabs[this->selectedtab]->button.back.x2;
            this->r_upb.p1 = this->m_upb.p2;
        }
    }
    else
    {
        this->selected = this->SearchControls(gx, gy);

        if (this->selected != nullptr)
        {
            switch (this->selected->type)
            {
            case Controls::ControlType::TextEditor:
                this->l_editors[this->selected->index]->DblClick(gx, gy);
                this->HideAllChilds();
                break;
            }
        }
    }
}

void Editor::processcoord(float& x)
{
    x = x + 1.0f;
    x = x * (glWIDTH / 2.0f);
    x = x - 1.0f;
}

void Editor::resize(float height, float width)
{
    
}

bool Editor::enterFullscreen(HWND hwnd)
{
    DEVMODE fullscreenSettings;
    bool isChangeSuccessful;
    RECT windowBoundary{};

    HDC windowHDC = GetDC(hwnd);
    int fullscreenWidth = GetDeviceCaps(windowHDC, DESKTOPHORZRES);
    int fullscreenHeight = GetDeviceCaps(windowHDC, DESKTOPVERTRES) - 1;
    int colourBits = GetDeviceCaps(windowHDC, BITSPIXEL);
    int refreshRate = GetDeviceCaps(windowHDC, VREFRESH);


    EnumDisplaySettings(NULL, 0, &fullscreenSettings);
    fullscreenSettings.dmPelsWidth = fullscreenWidth;
    fullscreenSettings.dmPelsHeight = fullscreenHeight;
    fullscreenSettings.dmBitsPerPel = colourBits;
    fullscreenSettings.dmDisplayFrequency = refreshRate;
    fullscreenSettings.dmFields = DM_PELSWIDTH |
        DM_PELSHEIGHT |
        DM_BITSPERPEL |
        DM_DISPLAYFREQUENCY;

    SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
    isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
    ShowWindow(hwnd, SW_SHOW);

    WIDTH  = (float)fullscreenWidth;
    HEIGHT = (float)fullscreenHeight;

    //this->resize(WIDTH, HEIGHT);

    return isChangeSuccessful; 
}

bool Editor::exitFullscreen(HWND hwnd)
{
    bool isChangeSuccessful;

    SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_LEFT);
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    isChangeSuccessful = ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;
    SetWindowPos(hwnd, HWND_NOTOPMOST, 200, 200, 1000, 1000, SWP_SHOWWINDOW);
    ShowWindow(hwnd, SW_RESTORE);

    return isChangeSuccessful;
}

void Editor::LoadLayout()
{
    this->AddButton(-0.93f, 0.98f, 0.12f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"File",            Color(1.0f, 1.0f, 1.0f), Center, 0);
    this->AddButton(-0.80f, 0.98f, 0.15f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Project",         Color(1.0f, 1.0f, 1.0f), Center, 0);
    this->AddButton(-0.64f, 0.98f, 0.12f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"View",            Color(1.0f, 1.0f, 1.0f), Center, 0);
    this->AddButton(-0.50f, 0.98f, 0.13f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Run",             Color(1.0f, 1.0f, 1.0f), Center, 0, 8);
    this->AddButton(-0.22f, 0.98f, 0.10f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Pip",             Color(1.0f, 1.0f, 1.0f), Center, 0);
    this->AddButton(-0.10f, 0.98f, 0.15f, 0.04f, Color(1.0f,  0.12f, 0.12f), &this->font, L"Designer",        Color(1.0f, 1.0f, 1.0f), Center, 0);
                                                                                                                                               
    this->AddButton(-0.92f, 0.93f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"New",             Color(1.0f, 1.0f, 1.0f), Left,   0,  0, false, 0);
    this->AddButton(-0.92f, 0.88f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Open",            Color(1.0f, 1.0f, 1.0f), Left,   0,  0, false, 0);
    this->AddButton(-0.92f, 0.83f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Save",            Color(1.0f, 1.0f, 1.0f), Left,   0,  0, false, 0);
    this->AddPanel (-0.93f, 0.939f, 0.30f, 0.156f, Color(0.12f, 0.12f, 0.12f), false, 0);                                                    
                                                                                                                                             
    this->AddButton(-0.62f, 0.93f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Item",            Color(1.0f, 1.0f, 1.0f), Left,   0,  1, false, 6);
    this->AddButton(-0.62f, 0.88f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Project",         Color(1.0f, 1.0f, 1.0f), Left,   0,  2, false, 6);
    this->AddPanel(-0.63f, 0.939f, 0.30f, 0.106f, Color(0.12f, 0.12f, 0.12f), false, 6);                                                     
                                                                                                                                             
    this->AddButton(-0.62f, 0.88f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Item",            Color(1.0f, 1.0f, 1.0f), Left,   0,  3, false, 7);
    this->AddButton(-0.62f, 0.83f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Folder",          Color(1.0f, 1.0f, 1.0f), Left,   0,  4, false, 7);
    this->AddButton(-0.62f, 0.78f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Project",         Color(1.0f, 1.0f, 1.0f), Left,   0,  5, false, 7);
    this->AddPanel(-0.63f, 0.889f, 0.30f, 0.156f, Color(0.12f, 0.12f, 0.12f), false, 7);                                                      
                                                                                                                                              
    this->AddButton(-0.62f, 0.83f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Save",            Color(1.0f, 1.0f, 1.0f), Left,   0,  6, false, 8);
    this->AddButton(-0.62f, 0.78f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Save All",        Color(1.0f, 1.0f, 1.0f), Left,   0,  7, false, 8);
    this->AddPanel(-0.63f, 0.839f, 0.30f, 0.106f, Color(0.12f, 0.12f, 0.12f), false, 8);                                                       
                                                                                                                                               
    this->AddButton(-0.79f, 0.93f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Scan",            Color(1.0f, 1.0f, 1.0f), Left,   0,  0, false, 1);
    this->AddButton(-0.79f, 0.88f, 0.28f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Settings",        Color(1.0f, 1.0f, 1.0f), Left,   0,  0, false, 1);
    this->AddPanel(-0.80f, 0.939f, 0.30f, 0.106f, Color(0.12f, 0.12f, 0.12f), false, 3);                                                     

    this->AddButton(-0.63f, 0.93f, 0.30f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Shell",           Color(1.0f, 1.0f, 1.0f), Left,   0,  0,  false, 2);
    this->AddButton(-0.63f, 0.88f, 0.30f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Project Explorer",Color(1.0f, 1.0f, 1.0f), Left,   0,  13, false, 2);
    this->AddPanel(-0.64f, 0.939f, 0.32f, 0.106f, Color(0.12f, 0.12f, 0.12f), false, 2);                                                      
                                                                                                                                              
    this->AddButton(-0.31f,  0.93f, 0.28f, 0.04f, Color(1.0f, 0.12f, 0.12f), &this->font, L"Small shell",     Color(1.0f, 1.0f, 1.0f), Left,   0,  0, false, 23);
    this->AddButton(-0.31f, 0.88f, 0.28f, 0.04f, Color(1.0f, 0.12f, 0.12f), &this->font, L"Big shell",        Color(1.0f, 1.0f, 1.0f), Left,   0,  0, false, 23);
    this->AddPanel(-0.32f, 0.939f, 0.30f, 0.106f, Color(0.12f, 0.12f, 0.12f), false, 23);

    this->AddButton(glWIDTH - 1.10f, 0.995f, 0.05f, 0.04f, Color(1.0f, 0.12f, 0.12f), &this->font, L"-", Color(1.0f, 1.0f, 1.0f), Center, 0, 10, true, -1);
    this->AddButton(glWIDTH - 1.05f, 0.995f, 0.05f, 0.04f, Color(1.0f, 0.12f, 0.12f), &this->font, L"x", Color(1.0f, 1.0f, 1.0f), Center, 0, 11, true, -1);

    this->AddButton(-0.99, 0.84, 0.3, 0.04, Color(0.12, 0.12, 0.12), &this->font, L"Project Explorer", Color(1.0, 1.0, 1.0), Left, 0, 10);
    this->l_buttons[this->l_buttons.size() - 1].text.orientation = 1;
    this->l_buttons[this->l_buttons.size() - 1].SizeToText();
    this->l_buttons[this->l_buttons.size() - 1].visible = false;

    this->AddButton(-0.99, 0.52, 0.3, 0.04, Color(0.12, 0.12, 0.12), &this->font, L"Toolbox", Color(1.0, 1.0, 1.0), Left, 0, 13);
    this->l_buttons[this->l_buttons.size() - 1].text.orientation = 1;
    this->l_buttons[this->l_buttons.size() - 1].SizeToText();
    this->l_buttons[this->l_buttons.size() - 1].visible = false;

    this->AddButton(-0.36f, 0.98f, 0.10f, 0.04f, Color(0.12f, 0.12f, 0.12f), &this->font, L"Stop", Color(1.0f, 1.0f, 1.0f), Center, 0, 12);

    this->AddButton(-0.10f, 0.93f, 0.15f, 0.04f, Color(1.0f, 0.12f, 0.12f), &this->font, L"Redesign", Color(1.0f, 1.0f, 1.0f), Left, 0, 15, false, 5);
    this->AddPanel(-0.10f, 0.93f, 0.16f, 0.042f, Color(0.12f, 0.12f, 0.12f), false, 5);

    this->exp.SetFont(&this->font);
    this->exp.SetSize(-0.95, 0.84, 0.7, 1.79, Color(0.18f, 0.18f, 0.18f));

    this->toolbox.Set(-0.95, 0.84, 0.7, 1.79, Color(0.18f, 0.18f, 0.18f), &this->font);
    this->toolbox.SetWinSize(WIDTH, HEIGHT);
    this->toolbox.SetButtons();
    this->toolbox.visible = false;

    this->h_button.Set(Color(0.9, 0.9, 0.9));
    this->h_button.outlined = true;

    this->im.LoadTexture("cute.png");
    this->im.Set(-0.98, 0.985, 0.05, 0.05);

    this->New_File_Chooser.Set(-0.25, 0.6, glWIDTH - 1.5, 1.2, Color(0.12, 0.12, 0.12), Color(0.44313725490, 0.37647058824, 0.90980392157), Color(1.0, 1.0, 1.0), &this->font);
    this->New_Project_Chooser.Set(-0.25, 0.6, glWIDTH - 1.5, 1.2, Color(0.12, 0.12, 0.12), Color(0.44313725490, 0.37647058824, 0.90980392157), Color(1.0, 1.0, 1.0), &this->font);
}

void Editor::AddButton(float x, float y, float width, float height, Color c, Font* vfont, wstring text, Color tc, Anchor an, float z, int command, bool visible, int parent)
{
    this->l_buttons.emplace_back(Button());
    this->l_buttons[this->l_buttons.size() - 1].Set(x, y, width, height, c, vfont, an, z);
    this->l_buttons[this->l_buttons.size() - 1].SetText(text, tc);
    this->l_buttons[this->l_buttons.size() - 1].visible = visible;

    this->controls.emplace_back(Controls::Control());
    this->controls[this->controls.size() - 1].index = (int)this->l_buttons.size() - 1;
    this->controls[this->controls.size() - 1].ID = parent;
    this->controls[this->controls.size() - 1].type = Controls::Button;
    this->controls[this->controls.size() - 1].command = command;
}

void Editor::AddPanel(float x, float y, float width, float height, Color c, bool visible, int parent)
{
    PopupPanel* p = new PopupPanel();
    this->l_panels.emplace_back(p);
    this->l_panels[this->l_panels.size() - 1]->Set(x, y, width, height, c);
    this->l_panels[this->l_panels.size() - 1]->visible = visible;

    this->controls.emplace_back(Controls::Control());
    this->controls[this->controls.size() - 1].index   = (int)this->l_panels.size() - 1;
    this->controls[this->controls.size() - 1].ID      = parent;
    this->controls[this->controls.size() - 1].type    = Controls::Panel;
}

void Editor::PESwitch()
{
    if (!this->exp.visible)
    {
        this->exp.visible = true;
        for (int i = 0; i < this->l_editors.size(); i++)
        {
            this->l_editors[i]->border.x1 = this->l_editors[i]->border.x1 + (this->exp.back.x2 - this->exp.back.x1);
            this->l_editors[i]->w_border.x1 = this->l_editors[i]->w_border.x1 + (this->exp.back.x2 - this->exp.back.x1);
        }
        for (int i = 0; i < this->tabs.size(); i++)
        {
            this->tabs[i]->button.back.x1 = this->tabs[i]->button.back.x1 + (this->exp.back.x2 - this->exp.back.x1);
            this->tabs[i]->button.back.x2 = this->tabs[i]->button.back.x2 + (this->exp.back.x2 - this->exp.back.x1);
            this->tabs[i]->button.text.x = this->tabs[i]->button.text.x + (this->exp.back.x2 - this->exp.back.x1);
        }
        this->l_upb.p1 = this->l_upb.p1 + (this->exp.back.x2 - this->exp.back.x1);
        this->l_upb.p2 = this->l_upb.p2 + (this->exp.back.x2 - this->exp.back.x1);
        this->m_upb.p1 = this->m_upb.p1 + (this->exp.back.x2 - this->exp.back.x1);
        this->m_upb.p2 = this->m_upb.p2 + (this->exp.back.x2 - this->exp.back.x1);
        this->r_upb.p1 = this->r_upb.p1 + (this->exp.back.x2 - this->exp.back.x1);
        for (auto& d : this->l_designers)
        {
            d.transx = 0.7f;
        }
    }
    else
    {
        this->exp.visible = false;
        for (int i = 0; i < this->l_editors.size(); i++)
        {
            this->l_editors[i]->border.x1 = this->l_editors[i]->border.x1 - (this->exp.back.x2 - this->exp.back.x1);
            this->l_editors[i]->w_border.x1 = this->l_editors[i]->w_border.x1 - (this->exp.back.x2 - this->exp.back.x1);
        }
        for (int i = 0; i < this->tabs.size(); i++)
        {
            this->tabs[i]->button.back.x1 = this->tabs[i]->button.back.x1 - (this->exp.back.x2 - this->exp.back.x1);
            this->tabs[i]->button.back.x2 = this->tabs[i]->button.back.x2 - (this->exp.back.x2 - this->exp.back.x1);
            this->tabs[i]->button.text.x = this->tabs[i]->button.text.x - (this->exp.back.x2 - this->exp.back.x1);
        }
        this->l_upb.p1 = this->l_upb.p1 - (this->exp.back.x2 - this->exp.back.x1);
        this->l_upb.p2 = this->l_upb.p2 - (this->exp.back.x2 - this->exp.back.x1);
        this->m_upb.p1 = this->m_upb.p1 - (this->exp.back.x2 - this->exp.back.x1);
        this->m_upb.p2 = this->m_upb.p2 - (this->exp.back.x2 - this->exp.back.x1);
        this->r_upb.p1 = this->r_upb.p1 - (this->exp.back.x2 - this->exp.back.x1);
        for (auto& d : this->l_designers)
        {
            d.transx = 0.0f;
        }
    }
}

void Editor::ToolBoxSwitch()
{
    if (!this->toolbox.visible)
    {
        this->toolbox.visible = true;
        for (int i = 0; i < this->l_editors.size(); i++)
        {
            this->l_editors[i]->border.x1 = this->l_editors[i]->border.x1 + (this->toolbox.back.x2 - this->toolbox.back.x1);
            this->l_editors[i]->w_border.x1 = this->l_editors[i]->w_border.x1 + (this->toolbox.back.x2 - this->toolbox.back.x1);
        }
        for (int i = 0; i < this->tabs.size(); i++)
        {
            this->tabs[i]->button.back.x1 = this->tabs[i]->button.back.x1 + (this->toolbox.back.x2 - this->toolbox.back.x1);
            this->tabs[i]->button.back.x2 = this->tabs[i]->button.back.x2 + (this->toolbox.back.x2 - this->toolbox.back.x1);
            this->tabs[i]->button.text.x = this->tabs[i]->button.text.x + (this->toolbox.back.x2 - this->toolbox.back.x1);
        }
        this->l_upb.p1 = this->l_upb.p1 + (this->toolbox.back.x2 - this->toolbox.back.x1);
        this->l_upb.p2 = this->l_upb.p2 + (this->toolbox.back.x2 - this->toolbox.back.x1);
        this->m_upb.p1 = this->m_upb.p1 + (this->toolbox.back.x2 - this->toolbox.back.x1);
        this->m_upb.p2 = this->m_upb.p2 + (this->toolbox.back.x2 - this->toolbox.back.x1);
        this->r_upb.p1 = this->r_upb.p1 + (this->toolbox.back.x2 - this->toolbox.back.x1);
        for (auto& d : this->l_designers)
        {
            d.transx = 0.7f;
        }
    }
    else
    {
        this->toolbox.visible = false;
        for (int i = 0; i < this->l_editors.size(); i++)
        {
            this->l_editors[i]->border.x1 = this->l_editors[i]->border.x1 - (this->toolbox.back.x2 - this->toolbox.back.x1);
            this->l_editors[i]->w_border.x1 = this->l_editors[i]->w_border.x1 - (this->toolbox.back.x2 - this->toolbox.back.x1);
        }
        for (int i = 0; i < this->tabs.size(); i++)
        {
            this->tabs[i]->button.back.x1 = this->tabs[i]->button.back.x1 - (this->toolbox.back.x2 - this->toolbox.back.x1);
            this->tabs[i]->button.back.x2 = this->tabs[i]->button.back.x2 - (this->toolbox.back.x2 - this->toolbox.back.x1);
            this->tabs[i]->button.text.x = this->tabs[i]->button.text.x - (this->toolbox.back.x2 - this->toolbox.back.x1);
        }
        this->l_upb.p1 = this->l_upb.p1 - (this->toolbox.back.x2 - this->toolbox.back.x1);
        this->l_upb.p2 = this->l_upb.p2 - (this->toolbox.back.x2 - this->toolbox.back.x1);
        this->m_upb.p1 = this->m_upb.p1 - (this->toolbox.back.x2 - this->toolbox.back.x1);
        this->m_upb.p2 = this->m_upb.p2 - (this->toolbox.back.x2 - this->toolbox.back.x1);
        this->r_upb.p1 = this->r_upb.p1 - (this->toolbox.back.x2 - this->toolbox.back.x1);
        for (auto& d : this->l_designers)
        {
            d.transx = 0.0f;
        }
    }
}

Controls::Control* Editor::SearchControls(float x, float y)
{
    for (int i = 0; i < this->controls.size(); i++)
    {
        switch (this->controls[i].type)
        {
        case Controls::ControlType::Button:
            if (this->l_buttons[this->controls[i].index].visible
                && this->l_buttons[this->controls[i].index].back.x1 < x
                && this->l_buttons[this->controls[i].index].back.x2 > x
                && this->l_buttons[this->controls[i].index].back.y1 > y
                && this->l_buttons[this->controls[i].index].back.y2 < y)
            {
                this->count_temp = i;
                return (&this->controls[i]);
            }
            break;
        case Controls::ControlType::TextEditor:
            if (this->l_editors[this->controls[i].index]->visible 
                && this->l_editors[this->controls[i].index]->border.x1 < x
                && this->l_editors[this->controls[i].index]->border.x2 > x
                && this->l_editors[this->controls[i].index]->border.y1 > y
                && this->l_editors[this->controls[i].index]->border.y2 < y)
            {
                return (&this->controls[i]);
            }
            break;
        case Controls::ControlType::Designer:
            if (this->l_designers[this->controls[i].index].visible
                && this->l_designers[this->controls[i].index].x1 < x
                && this->l_designers[this->controls[i].index].x2 > x
                && this->l_designers[this->controls[i].index].y1 > y
                && this->l_designers[this->controls[i].index].y2 < y)
            {
                return (&this->controls[i]);
            }
            break;
        case Controls::ControlType::Window:

            break;
        }
    }
    this->count_temp = -1;
    return nullptr;
}

void Editor::ButtonClick(Controls::Control* id, int count)
{    
    //Hide all notconnected childs childs
    this->HideAllChilds();
    Controls::Control* parent = nullptr;
    parent = &this->controls[count];
    while (parent->ID != -1)
    {
        for (int i = 0; i < this->controls.size(); i++)
        {
            if (this->controls[i].ID == parent->ID)
            {
                this->ChangeVisible(&this->controls[i]);
            }
        }
        parent = &this->controls[parent->ID];
    }
    
    //Make all childs visible
    if (id->extr)
    {
        this->HideChilds(id, count);
    }
    else
    {
        //Show childs
        for (int i = count + 1; i < this->controls.size(); i++)
        {
            if (this->controls[i].ID == count)
            {
                this->ChangeVisible(&this->controls[i]);
            }
        }
        id->extr = true;
    }

    if (id->command != 0)
        this->HideAllChilds();

    switch (id->command)
    {
    case 1:
        this->NewFile();
        break;
    case 2: 
        this->NewProject();
        break;
    case 3:
        this->OpenFile();
        break;
    case 4:
       
        break;
    case 5: 
        this->OpenProject();
        break;
    case 6:
        this->SaveFile();
        break;
    case 7:
        this->SaveAll();
        break;
    case 8:
        this->Run();
        break;
    case 9:
        if (this->toolbox.visible)
            this->ToolBoxSwitch();
        this->PESwitch();
        break;
    case 10:
        ShowWindow(*this->target, SW_MINIMIZE);
        break;
    case 11: 
        this->debugger->End();
        PostQuitMessage(0);
        break;
    case 12:
        this->debugger->Stop();
        break;
    case 13:
        if (!this->l_buttons[this->l_buttons.size() - 3].visible)
        {
            this->l_buttons[this->l_buttons.size() - 3].visible = true;
        }
        if (this->exp.visible = false)
        {
            this->PESwitch();
        }
        break;
    case 14:
        if (this->exp.visible)
            this->PESwitch();
        this->ToolBoxSwitch();
        break;
    case 15:
    {
        for (int i = 0; i < this->tabs.size(); i++)
        {
            if (this->tabs[i]->holdtype == pDesignerFile)
            {
                this->l_designers[this->tabs[i]->index].ReDesign();
            }
        }
    }
    }
}

void Editor::HideChilds(Controls::Control* id, int count)
{
    for (int i = count + 1; i < this->controls.size(); i++)
    {
        if (this->controls[i].ID == count)
        {
            this->HideChilds(&this->controls[i], i);
            switch (this->controls[i].type)
            {
            case Controls::ControlType::Button:
                this->l_buttons[this->controls[i].index].visible = false;
                break;
            case Controls::ControlType::Panel:
                this->l_panels[this->controls[i].index]->visible = false;
                break;
            }
        }
    }
    id->extr = false;
}

void Editor::HideAllChilds()
{
    this->h_button.visible = false;
    for (int i = 0; i < this->controls.size(); i++)
    {
        this->controls[i].extr = false;
        if (this->controls[i].ID > -1)
        {
            switch (this->controls[i].type)
            {
            case Controls::ControlType::Button:
                this->l_buttons[this->controls[i].index].visible = false;
                break;
            case Controls::ControlType::Panel:
                this->l_panels[this->controls[i].index]->visible = false;
                break;
            }
        }
    }
}

void Editor::ChangeVisible(Controls::Control* id)
{
    switch (id->type)
    {
    case Controls::ControlType::Button:
        this->l_buttons[id->index].visible = !this->l_buttons[id->index].visible;
        break;
    case Controls::ControlType::Panel:
        this->l_panels[id->index]->visible = !this->l_panels[id->index]->visible;
        break;
    }
}

void Editor::NewFile()
{
    this->Procedure = 1;
    this->New_File_Chooser.active = true;
}

void Editor::CreateNewFile(wstring name)
{
    TextEditor* ed = new TextEditor();
    ed->Set(-0.9, 0.8, glWIDTH - 0.2, 1.75, Color(0.12f, 0.12f, 0.12f), &this->font);
    ed->smart = &smart;
    ed->Add(L"Write your code here...", true);
    ed->Current_Line = ed->Lines.size() - 1;
    ed->Token_Iterator = ed->Lines[ed->Current_Line].tokens.size() - 1;
    ed->Letter_Iterator = ed->Lines[ed->Current_Line].tokens[ed->Lines[ed->Current_Line].tokens.size() - 1].token.size();
    ed->CursorOnEnd();
    this->l_editors.push_back(ed);

    Controls::Control con;
    con.index = this->l_editors.size() - 1;
    con.ID = this->controls.size();
    con.type = Controls::TextEditor;
    con.command = 0;
    this->controls.push_back(con);

    Tab* tab = new Tab();
    if (this->tabs.size() > 0)
    {
        tab->button.Set(this->tabs[this->tabs.size() - 1]->button.back.x2 + 0.004, 0.84f, 0.0f, 0.04f, Color(0.18f, 0.18f, 0.18f), &this->font, Left);
    }
    else
    {
        tab->button.Set(-0.90f, 0.84f, 0.0f, 0.04f, Color(0.18f, 0.18f, 0.18f), &this->font, Left);
    }
    tab->linked_to_file = false;
    tab->button.SetText(name, Color(1.0f, 1.0f, 1.0f), true);
    tab->button.visible = true;
    tab->holdtype = UnsavedNewFile;
    tab->index = this->l_editors.size() - 1;
    this->tabs.push_back(tab);
    this->selectedtab = this->tabs.size() - 1;
    for (auto& each : this->tabs)
        each->button.back.qolor = Color(0.18f, 0.18f, 0.18f);
    this->tabs[this->selectedtab]->button.back.qolor = Color(0.2f, 0.2f, 0.2f);

    this->selected = &this->controls[this->controls.size() - 1];

    for (auto& each : this->l_editors)
        each->visible = false;
    for (auto& each : this->l_designers)
        each.visible = false;
    this->l_editors[this->l_editors.size() - 1]->visible = true;

    this->l_upb.Set(-0.90f, +0.80f, -0.90f, +0.80f, Color(0.44313725490, 0.37647058824, 0.90980392157));
    this->m_upb.Set(-0.90f, +0.84f, -0.90f, +0.84f, Color(0.44313725490, 0.37647058824, 0.90980392157));
    this->r_upb.Set(+glWIDTH - 1.1f, +0.80f, +glWIDTH - 1.1f, +0.80f, Color(0.44313725490, 0.37647058824, 0.90980392157));

    this->l_upb.p2 = this->tabs[this->selectedtab]->button.back.x1;
    this->m_upb.p1 = this->l_upb.p2;              
    this->m_upb.p2 = this->tabs[this->selectedtab]->button.back.x2;
    this->r_upb.p1 = this->m_upb.p2;
}

void Editor::CreateNewProject(wstring name, int type)
{
    string path = string(this->New_Project_Chooser.directory.data.data.begin(), this->New_Project_Chooser.directory.data.data.end());
    if (path[path.size() - 1] != '\\')
        path.push_back('\\');
    this->SaveAll();    
    this->project.CreateProject(path, string(name.begin(), name.end()), ProjectType(type));
    this->OpenProject(wstring(path.begin(), path.end()) + name + L".pyproj");
}

void Editor::NewProject()
{
    this->Procedure = 2;
    this->New_Project_Chooser.active = true;
}

size_t GetSizeOfFile(const std::wstring& path)
{
    struct _stat fileinfo;
    _wstat(path.c_str(), &fileinfo);
    return fileinfo.st_size;
}

void Editor::OpenFile()
{
    OPENFILENAME ofn;
    wchar_t fileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = *this->target;
    ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"";
    wstring filename;
    if (GetOpenFileName(&ofn))
    {
        filename = fileName;
        this->OpenFile(filename, -1, false);
    }
}

void Editor::OpenFile(wstring path, int i, bool inproject)
{
    TextEditor* ed = new TextEditor();
    ed->Set(-0.9, 0.8, glWIDTH - 0.2, 1.75, Color(0.12f, 0.12f, 0.12f), &this->font);
    ed->smart = &smart;

    std::wifstream stream(path);
    stream.imbue(std::locale("zh_CN.UTF-8"));
    std::wstring line;

    int ind = 0;
    bool first = true;
    while (getline(stream, line))
    {
        /*if (line.size() == 0)
        {
            ind = ind + 1;
        }
        else
        {
            if (first)
            {
                first = false;
            }
            else
            {
                ed->Enter();
            }
            for (int i = 0; i < ind; i++)
                ed->Enter();
            ind = 0;
            if (ed->Lines.size() == 0)
            {
                TLine line;
                ed->Lines.push_back(line);
                ed->Current_Line = 0;
                TToken token;
                token.type = Blank;
                ed->Lines[ed->Current_Line].tokens.push_back(token);
                ed->Token_Iterator = 0;
                ed->Letter_Iterator = 0;
            }
            ed->Lines[ed->Current_Line].tokens[0].token= line;
            std::thread tokenizer(TokenizeLine, &ed->Lines[ed->Current_Line]);
            tokenizer.detach();
            //ed->Add(line, true);   */

    TLine l;
    l.size = line.size();
    l.tokens.push_back(TToken(line, Plain));
    ed->Lines.push_back(l);
    ed->Current_Line = ed->Lines.size() - 1;
    }
    /*for (int i = 0; i < ed->Lines.size(); i++)
    {
        for (int j = 0; j < ed->Lines[i].tokens.size() - 1; j++)
        {
            wcout << ed->Lines[i].tokens[j].token << endl;
        }
    }*/
    if (ed->Lines.size() > 0)
    {
        ed->Current_Line = ed->Lines.size() - 1;
        ed->Token_Iterator = ed->Lines[ed->Current_Line].tokens.size() - 1;
        ed->Letter_Iterator = ed->Lines[ed->Current_Line].tokens[ed->Lines[ed->Current_Line].tokens.size() - 1].token.size();
        ed->CursorOnEnd();
    }

    stream.close();
    this->l_editors.push_back(ed);
    bool desf = false;
    for (auto& each : this->project.projdesignerfiles)
    {
        if (each == i)
        {
            this->OpenDesigner(this->l_editors[this->l_editors.size() - 1]);
            this->l_editors[this->l_editors.size() - 1]->SetDesignerBelongine(this->l_designers.size() - 1);
            desf = true;
        }
    }
    if (desf)
    {
        std::thread tokenizer(TokenizeLine, &this->l_editors[this->l_editors.size() - 1]->Lines, &this->l_designers[this->l_designers.size()-1]);
        tokenizer.detach();
    }
    else
    {
        std::thread tokenizer(TokenizeLine, &this->l_editors[this->l_editors.size() - 1]->Lines, nullptr);
        tokenizer.detach();
    }

    Controls::Control con;
    con.index = this->l_editors.size() - 1;
    con.ID = this->controls.size();
    con.type = Controls::TextEditor;
    con.command = 0;
    this->controls.push_back(con);

    Tab* tab = new Tab();
    if (this->tabs.size() > 0)
    {
        tab->button.Set(this->tabs[this->tabs.size() - 1]->button.back.x2 + 0.004, 0.84f, 0.0f, 0.04f, Color(0.18f, 0.18f, 0.18f), &this->font, Left);
    }
    else
    {
        tab->button.Set(-0.90f, 0.84f, 0.0f, 0.04f, Color(0.18f, 0.18f, 0.18f), &this->font, Left);
    }
    tab->path_to_file = path;
    tab->linked_to_file = true;
    int off = path.find_last_of('\\');
    path = path.substr(off + 1, path.size() - off - 1);
    tab->button.SetText(path, Color(1.0f, 1.0f, 1.0f), true);
    tab->button.visible = true;
    if (inproject == true)
        tab->holdtype = ProjectFile;
    else
        tab->holdtype = OpenedFile;
    tab->index = this->l_editors.size() - 1;
    this->tabs.push_back(tab);
    this->selectedtab = this->tabs.size() - 1;
    for (auto& each : this->tabs)
        each->button.back.qolor = Color(0.18f, 0.18f, 0.18f);
    this->tabs[this->selectedtab]->button.back.qolor = Color(0.2f, 0.2f, 0.2f);

    this->selected = &this->controls[this->controls.size() - 1];

    for (auto& each : this->l_editors)
        each->visible = false;
    for (auto& each : this->l_designers)
        each.visible = false;
    this->l_editors[this->l_editors.size() - 1]->visible = true;

    if (!inproject)
    {
        this->l_upb.Set(-0.90f,          +0.80f, -0.90f,          +0.80f, Color(0.44313725490, 0.37647058824, 0.90980392157));
        this->m_upb.Set(-0.90f,          +0.84f, -0.90f,          +0.84f, Color(0.44313725490, 0.37647058824, 0.90980392157));
        this->r_upb.Set(+glWIDTH - 1.1f, +0.80f, +glWIDTH - 1.1f, +0.80f, Color(0.44313725490, 0.37647058824, 0.90980392157));

        this->l_upb.p2 = this->tabs[this->selectedtab]->button.back.x1;
        this->m_upb.p1 = this->l_upb.p2; 
        this->m_upb.p2 = this->tabs[this->selectedtab]->button.back.x2;
        this->r_upb.p1 = this->m_upb.p2;
    }

    //Open designer
    
    //this->l_editors[this->l_editors.size() - 1]->changedlines.clear();
}

void Editor::OpenDesigner(TextEditor* editor)
{
    DesignerW des;
    des.Set(&this->font, WIDTH, HEIGHT, glWIDTH, -0.9, 0.8, &this->definedcolors, &this->typenames);
    des.x2 = glWIDTH - 1;
    des.y2 = -1;
    des.SetEditor(editor);
    this->l_designers.push_back(des);

    Controls::Control con;
    con.index = this->l_designers.size() - 1;
    con.ID = this->controls.size();
    con.type = Controls::Designer;
    con.command = 0;
    this->controls.push_back(con);

    Tab* tab = new Tab();
    tab->button.Set(this->tabs[this->tabs.size() - 1]->button.back.x2 + 0.004f, 0.84f, 0.0f, 0.04f, Color(0.24f, 0.24f, 0.24f), &this->font, Left);
    wstring path = this->tabs[this->tabs.size() - 1]->button.text.data + L" - D";
    tab->button.SetText(path, Color(1.0f, 1.0f, 1.0f), true);
    tab->button.visible = true;
    tab->holdtype = pDesignerFile;
    tab->index = this->l_designers.size() - 1;
    this->tabs.push_back(tab);
    this->selectedtab = this->tabs.size() - 1;
    for (auto& each : this->tabs)
        each->button.back.qolor = Color(0.24f, 0.24f, 0.24f);
    this->tabs[this->selectedtab]->button.back.qolor = Color(0.3f, 0.3f, 0.3f);
    this->selected = &this->controls[this->controls.size() - 1];

    for (auto& each : this->l_editors)
        each->visible = false;
    for (auto& each : this->l_designers)
        each.visible = false;
    this->l_designers[this->l_designers.size()-1].visible = true;
}

void Editor::OpenProject()
{
    OPENFILENAME ofn;
    wchar_t fileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = *this->target;
    ofn.lpstrFilter = L"All Files (*.pyproj*)\0*.pyproj*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"";
    wstring filename;
    if (GetOpenFileName(&ofn))
    {
        filename = fileName;
        this->OpenProject(filename);
    }
}

void Editor::OpenProject(wstring path)
{
    this->l_upb.Set(-0.90f, +0.8f, -0.90f, +0.8f, Color(0.44313725490, 0.37647058824, 0.90980392157));
    this->m_upb.Set(-0.90f, +0.84f, -0.90f, +0.84f, Color(0.44313725490, 0.37647058824, 0.90980392157));
    this->r_upb.Set(+glWIDTH - 1.1f, +0.8f, +glWIDTH - 1.1f, +0.8f, Color(0.44313725490, 0.37647058824, 0.90980392157));

    this->tabs.clear();
    this->l_editors.clear();
    this->l_designers.clear();

    this->project.LoadProject(string(path.begin(), path.end()));
    for (int i = 0; i < this->project.projfiles.size(); i++)
    {
        this->OpenFile(wstring(this->project.settings.projpath.begin(), this->project.settings.projpath.end()) +
            wstring(this->project.projfiles[i].begin(), this->project.projfiles[i].end()), i, true);
    }
    this->exp.visible = false;
    this->exp.DispProj(&this->project);

    this->l_buttons[this->l_buttons.size() - 3].visible = true;
    this->l_buttons[this->l_buttons.size() - 2].visible = true;

    this->l_upb.p2 = this->tabs[this->selectedtab]->button.back.x1;
    this->m_upb.p1 = this->l_upb.p2;
    this->m_upb.p2 = this->tabs[this->selectedtab]->button.back.x2;
    this->r_upb.p1 = this->m_upb.p2;
}

void Editor::SaveFile()
{
    switch (this->tabs[this->selectedtab]->holdtype)
    {
    case OpenedFile:
    case ProjectFile:
    {
        std::ofstream ofs;
        ofs.open(this->tabs[this->selectedtab]->path_to_file, std::ofstream::out | std::ofstream::trunc);
        ofs.close();

        //Save file
        std::wofstream log(this->tabs[this->selectedtab]->path_to_file, std::ios_base::app | std::ios_base::out);
        for (auto& a : this->l_editors[this->selectedtab]->Lines)
        {
            for (auto& b : a.tokens)
                log << b.token;
            log << L"\n";
        }
        log.close();
        break;
    }
    case UnsavedNewFile:
    {
        OPENFILENAME ofn = { 0 };
        wstring w_name = this->tabs[this->selectedtab]->button.text.data;
        wchar_t* wc_name = (wchar_t*)w_name.c_str();
        const WCHAR* wn = w_name.c_str();

        ofn.lStructSize = sizeof(ofn);
        ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;
        ofn.hInstance = GetModuleHandle(0);
        ofn.hwndOwner = *this->target;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFile = (WCHAR*)(new wstring(wn))->c_str();
        ofn.nFilterIndex = 1;
        ofn.lpstrFilter = L"Python script\0*.py\0Any file\0*.*\0";
        ofn.lpstrDefExt = L"py";
        if (GetSaveFileName(&ofn))
        {
            std::ofstream ofs;
            ofs.open(ofn.lpstrFile, std::ofstream::out | std::ofstream::trunc);
            ofs.close();

            //Save file
            std::wofstream log(ofn.lpstrFile, std::ios_base::app | std::ios_base::out);
            for (auto& a : this->l_editors[this->selectedtab]->Lines)
            {
                for (auto& b : a.tokens)
                    log << b.token;
                log << L"\n";
            }
            log.close();

            wstring name = ofn.lpstrFile;
            
            this->tabs[this->selectedtab]->holdtype = OpenedFile;
            this->tabs[this->selectedtab]->button.text.data = name.substr(name.find_last_of('\\') + 1, name.size() - name.find_last_of('\\') - (name.size() - name.find_last_of('.')) - 1);
            this->tabs[this->selectedtab]->linked_to_file = true;
            this->tabs[this->selectedtab]->path_to_file = ofn.lpstrFile;
        }

        break;
    }
    case pDesignerFile:
    {

        break;
    }
    case fDesignerFile:
    {

        break;
    }
    }
}

void Editor::SaveAll()
{
    for (int i = 0; i < this->tabs.size(); i++)
    {
        for (auto& e : this->project.projfiles)
        {
            if (wstring(e.begin(), e.end()) == this->tabs[i]->button.text.data)
            {
                //Clear file
                std::ofstream ofs;
                ofs.open(this->project.settings.projpath + e, std::ofstream::out | std::ofstream::trunc);
                ofs.close();

                //Save file
                std::wofstream log(this->project.settings.projpath + e, std::ios_base::app | std::ios_base::out);
                for (auto& a : this->l_editors[i]->Lines)
                {
                    for (auto& b : a.tokens)
                        log << b.token;
                    log << L"\n";
                }
                log.close();
            }
        }
    }
}

void Editor::Run()
{
    if (this->tabs.size() > 0)
    {
        if (this->tabs[this->selectedtab]->holdtype == ProjectFile || this->tabs[this->selectedtab]->holdtype == pDesignerFile)
        {
            if (this->project.projfiles.size() > 0)
            {
                this->SaveAll();
                wstring pathdir = wstring(this->project.settings.projpath.begin(), this->project.settings.projpath.end())
                    + wstring(this->project.projfiles[0].begin(), this->project.projfiles[0].end());
                this->debugger->StartProgram(pathdir, this->project.settings.projpath);
            }
        }
        else if (this->tabs[this->selectedtab]->holdtype == OpenedFile)
        {
            int off = this->tabs[this->selectedtab]->path_to_file.find_last_of('\\');
            wstring workdir = this->tabs[this->selectedtab]->path_to_file.substr(off + 1, this->tabs[this->selectedtab]->path_to_file.size() - off - 1);
            system("cls");
            HWND chwnd = GetConsoleWindow();
            SetForegroundWindow(chwnd);
            this->debugger->StartProgram(this->tabs[this->selectedtab]->path_to_file, string(workdir.begin(), workdir.end()));
        }
    }
}

void Editor::ProcessLineChanges()
{
    if (this->selected != nullptr)
    {
        if (this->selected->type == Controls::ControlType::TextEditor)
        {
            if (ifDesignerTextEditor(this->selected->index))
            {
                //this->l_designers[this->l_editors[this->selected->index]->designernum].DesignerLineChanged(this->l_editors[this->selected->index]->changedlines);
            }
            else
            {
                //TODO Later
            }
            //this->l_editors[this->selected->index]->changedlines.clear();
        }
    }
}

bool Editor::ifDesignerTextEditor(int editor)
{
    if (this->l_editors[editor]->designernum > -1)
        return true;
    return false;
}

void TokenizeLine(vector<TLine>* lines, DesignerW* designer)
{
    for (int i = 0; i < (*lines).size(); i++)
    {

        TLine line;
        line.size = (*lines)[i].size;
        line.tokens.push_back(TToken(L"", Plain));
        int tokencount = 0;
        int tokeniter = 0;
        for (int j = 0; j < (*lines)[i].tokens[0].token.size(); j++)
        {
            wchar_t key = (*lines)[i].tokens[0].token[j];
            switch (key)
            {
            case 9:
            case ' ':
                if (line.tokens[tokencount].type == Characters)
                {
                    line.tokens[tokencount].token.push_back(key);
                    tokeniter++;
                }
                else
                {
                    if (tokencount > 0 && tokeniter == 0 && line.tokens[tokencount - 1].type == Blank)
                    {
                        line.tokens[tokencount - 1].token.push_back(key);
                    }
                    else if (line.tokens[tokencount].token.size() == 0)
                    {
                        line.tokens[tokencount].token.push_back(key);
                        line.tokens[tokencount].type = Blank;
                        line.tokens.push_back(TToken(L"", Plain));
                        tokencount++;
                        tokeniter = 0;
                    }
                    else
                    {
                        line.tokens.push_back(TToken(key, Blank));
                        line.tokens.push_back(TToken(L"", Plain));
                        tokencount = tokencount + 2;
                        tokeniter = 0;
                    }
                }
                break;
            case '\"':
            {
                if (line.tokens[tokencount].type == Characters)
                {
                    line.tokens[tokencount].token.push_back(key);
                    line.tokens.push_back(TToken(L"", Plain));
                    tokencount++;
                    tokeniter = 0;
                }
                else
                {
                    if (line.tokens[tokencount].token.size() == 0)
                    {
                        line.tokens.erase(line.tokens.begin() + tokencount);
                        tokencount--;
                    }
                    line.tokens.push_back(TToken(key, Characters));
                    tokencount++;
                    tokeniter = 1;                    
                }
                break;
            }
            case '(':
                if (line.tokens[tokencount].type == Characters)
                {
                    line.tokens[tokencount].token.push_back(key);
                    tokeniter++;
                }
                else
                {
                    line.tokens[tokencount].type = Function;
                    line.tokens.push_back(TToken(key, Symbol));
                    tokencount++;
                    line.tokens.push_back(TToken(L"", Plain));
                    tokencount++;
                    tokeniter = 0;
                }                
                break;
            case ')':
                if (line.tokens[tokencount].type == Characters)
                {
                    line.tokens[tokencount].token.push_back(key);
                    tokeniter++;
                }
                else
                {
                    if (line.tokens[tokencount].token.size() == 0)
                    {
                        line.tokens[tokencount].token.push_back(key);
                        line.tokens[tokencount].type = Symbol;
                    }
                    else
                    {
                        line.tokens.push_back(TToken(key, Symbol));
                        tokeniter++;
                    }
                    line.tokens.push_back(TToken(L"", Plain));
                    tokencount++;
                    tokeniter = 0;
                }               
                break;
            case '=':
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
                if (tokencount > 0 && tokeniter == 0 && line.tokens[tokencount - 1].type == Symbol)
                {
                    line.tokens[tokencount - 1].token.push_back(key);
                }
                else if (line.tokens[tokencount].token.size() == 0)
                {
                    line.tokens[tokencount].token.push_back(key);
                    line.tokens[tokencount].type = Symbol;
                    line.tokens.push_back(TToken(L"", Plain));
                    tokencount++;
                    tokeniter = 0;
                }
                else
                {
                    line.tokens.push_back(TToken(key, Symbol));
                    line.tokens.push_back(TToken(L"", Plain));
                    tokencount = tokencount + 2;
                    tokeniter = 0;
                }
                break;
            default:
                line.tokens[tokencount].token.push_back(key);
                tokeniter++;
                break;
            }
            //if (line.tokens[line.tokens.size() - 1].token.size() == 0)
                //line.tokens.erase(line.tokens.begin() + line.tokens.size() - 1);
        }
        (*lines)[i] = line;
    }
    if (designer != nullptr)
        designer->DesignFile();
}

SuggestionType TokenizeToken(wstring token, SmartSuggestion* smart)
{
    for (auto& each : smart->label.labels)
    {
        if (wstring(each.word.begin(), each.word.end()) == token)
        {
            return each.type;
        }
    }
    for (auto& each : smart->label.py_modules)
    {
        if (each == token)
        {
            return Import;
        }
    }
    //this->Lines[this->currentline].tokens[this->w_token].type = Plain;
    /*for (auto& each : this->imports)
    {
        if (each == this->Lines[this->Current_Line].tokens[this->Token_Iterator].token)
        {
            this->Lines[this->Current_Line].tokens[this->Token_Iterator].type = Import;
            return;
        }
    }*/
    /*for (auto& each : this->variables)
    {
        if (each == this->Lines[this->Current_Line].tokens[this->Token_Iterator].token)
        {
            this->Lines[this->Current_Line].tokens[this->Token_Iterator].type = Object;
            return;
        }
    }*/
    return Plain;
}
