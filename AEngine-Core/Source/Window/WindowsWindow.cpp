#include "WindowsWindow.h"

using namespace AE::System;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch (message)
    {
        // this message is read when the window is closed
    case WM_DESTROY:
    {
        // close the application entirely
        PostQuitMessage(0);
        return 0;
    } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}


void WindowsWindow::Init(WindowCreateInfo info) {
    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;

    //Could be a problem area only works if statically linked
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClassEx(&wc);

    //be able to set the client size properly
    RECT rect = { 0, 0, info.x, info.y };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    std::wstring windowNameWide = std::wstring(info.windowName.begin(), info.windowName.end());

    // create the window and use the result as the handle
    m_hwnd = CreateWindowEx(NULL,
        L"WindowClass1",    // name of the window class
        windowNameWide.c_str(),   // title of the window
        WS_OVERLAPPEDWINDOW,    // window style
        300,    // x-position of the window
        300,    // y-position of the window
        rect.right - rect.left,    // width of the window
        rect.bottom - rect.top,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        wc.hInstance,    // application handle
        NULL);    // used with multiple windows, NULL

    // display the window on the screen
    ShowWindow(m_hwnd, SW_NORMAL);
};

void WindowsWindow::ShutDown() {

};

bool WindowsWindow::ShouldEngineExit() {
    MSG msg;

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        // translate keystroke messages into the right format
        TranslateMessage(&msg);

        // send the message to the WindowProc function
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT)
            return true;
    }

    return false;
};

void WindowsWindow::Poll() {

};

void* WindowsWindow::GetWindowHandleImpl() {
    return m_hwnd;
}