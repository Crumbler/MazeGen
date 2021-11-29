#include "mazeControl.hpp"
#include "mazeControlFuncs.hpp"

#include <cstdio>

LRESULT CALLBACK MazeWndProc(HWND, UINT, WPARAM, LPARAM);

void RegisterMazeControl(HINSTANCE hInstance)
{
    WNDCLASSEXW wincl;

    wincl.hInstance = hInstance;
    wincl.lpszClassName = szMazeControl;
    wincl.lpfnWndProc = MazeWndProc;
    wincl.style = 0;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wincl.lpszMenuName = nullptr;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = nullptr;

    RegisterClassExW(&wincl);
}

LRESULT CALLBACK MazeWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        OnMazeCreate(hwnd);
        break;

    case WM_CLOSE:
        OnMazeClose(hwnd);
        break;

    case WM_ERASEBKGND:
        return 1;

    case WM_SIZE:
        OnMazeResize(hwnd, LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_PAINT:
        OnMazePaint(hwnd);
        break;

    case WM_GENERATE:
        OnMazeGenerate(hwnd, (Alg)wParam, lParam);
        break;

    case WM_SETVIS:
        OnSetVisualizing(hwnd, wParam);
        break;

    case WM_SAVEAS:
        OnMazeSaveAs(hwnd, (char*)lParam);
        break;

    case WM_LOAD:
        return OnMazeLoad(hwnd, (char*)lParam);

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
