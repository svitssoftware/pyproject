#include "Editor.h"
#include <iostream>
using namespace std;

Editor* editor;

HWND              hWnd;
MSG               msg;
HDC               hDC;
HGLRC             hRC;

UINT_PTR rentimer;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int main()//WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    HWND chwnd = GetConsoleWindow();
    HMENU chmenu = GetSystemMenu(chwnd, FALSE);
    //EnableMenuItem(chmenu, SC_CLOSE, MF_DISABLED);

    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_DBLCLKS | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"PyStudioEditor";
    RegisterClassEx(&wc);

    RECT lpRect;
    GetWindowRect(GetDesktopWindow(), &lpRect);
    lpRect.bottom = lpRect.bottom - 1;
    hWnd = CreateWindowEx(NULL,
        L"PyStudioEditor",
        L"PyStudio",
        WS_POPUP | WS_VISIBLE,
        lpRect.left,
        lpRect.top,
        lpRect.right  - lpRect.left,
        lpRect.bottom - lpRect.top,
        NULL,
        NULL,
        NULL,
        NULL);

    /*HWND consoleWindowHandle = GetConsoleWindow();
    SetWindowPos(
        consoleWindowHandle, // window handle
        HWND_TOPMOST, // "handle to the window to precede
                      // the positioned window in the Z order
                      // OR one of the following:"
                      // HWND_BOTTOM or HWND_NOTOPMOST or HWND_TOP or HWND_TOPMOST
        0, 0, // X, Y position of the window (in client coordinates)
        0, 0, // cx, cy => width & height of the window in pixels
        SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW // The window sizing and positioning flags.
    );
    // OPTIONAL ! - SET WINDOW'S "SHOW STATE"
    ShowWindow(
        consoleWindowHandle, // window handle
        SW_NORMAL // how the window is to be shown
                  // SW_NORMAL => "Activates and displays a window.
                  // If the window is minimized or maximized,
                  // the system restores it to its original size and position.
                  // An application should specify this flag
                  // when displaying the window for the first time."
    );*/

    editor = new Editor(&hWnd);

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    glClearColor(0.12f, 0.12f, 0.12f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    float glw = 2 * (float)((float)(lpRect.right / (float)(lpRect.bottom)));
    gluOrtho2D(-1.0, glw - 1, -1.0, 1.0);

    editor->Load(lpRect.right, lpRect.bottom, glw);
    //editor->LoadProject("C:\\test\\test.pyproj");

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    rentimer = SetTimer(hWnd, 0, 40, (TIMERPROC)NULL);

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnregisterClass(L"PyStudioEditor", NULL);
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT   ps;

    switch (message)
    {
    case WM_PAINT:
    {
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_TIMER:
    {        
        editor->Render();
        return 0;
    }
    case WM_CHAR:
    {
        editor->keyClick(wParam);
        return 0;
    }
    case WM_KEYDOWN:
    {    
        editor->keyDown(wParam);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        editor->mouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_LBUTTONUP:
    {
        editor->mouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_LBUTTONDBLCLK:
    {
        editor->mouseDblClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_MOUSEWHEEL:
    {
        editor->mouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        editor->mouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
