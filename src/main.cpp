#include <windows.h>
#include <gdiplus.h>

#include <iostream>

#include "windowFuncs.hpp"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

constexpr WCHAR szClassName[] = L"MazeGen";

HINSTANCE hInstanceMain;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszArgument, int nCmdShow)
{
    using Gdiplus::GdiplusStartupInput;
    using Gdiplus::GdiplusShutdown;

    HWND hwnd;
    WNDCLASSEXW wincl;

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    hInstanceMain = hInstance;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wincl.hInstance = hInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WndProc;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = nullptr;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    if (!RegisterClassExW(&wincl))
        return 0;

    hwnd = CreateWindowExW(
           0,
           szClassName,
           L"MazeGen",
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           1280,
           720,
           HWND_DESKTOP,
           nullptr,
           hInstance,
           nullptr);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        OnCreate(hwnd, hInstanceMain);
        break;

    case WM_ERASEBKGND:
        return 1;

    case WM_SIZE:
        OnResize(hwnd, LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_PAINT:
        OnPaint(hwnd);
        break;

    case WM_COMMAND:
        OnCommand(hwnd, wParam, lParam);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
