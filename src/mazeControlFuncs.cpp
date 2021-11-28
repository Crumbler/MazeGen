#include "mazeControlFuncs.hpp"
#include "mazeControl.hpp"

#include <cstdio>

#include <gdiplus.h>

using namespace Gdiplus;

MazeControlExtra* GetMazeControl(HWND hwnd)
{
    return (MazeControlExtra*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

void OnMazeCreate(HWND hwnd)
{
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)new MazeControlExtra());
}


void OnMazePaint(HWND hwnd)
{
    MazeControlExtra* mzCtrl = GetMazeControl(hwnd);

    int width = mzCtrl->width, height = mzCtrl->height;

    int scale;
    float trX = 0.0f, trY = 0.0f;

    if (width > height)
    {
        scale = height;
        trX = (width - height) / 2.0f;
    }
    else
    {
        scale = width;
        trY = (height - width) / 2.0f;
    }

    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);

    Graphics graphics(hdc);

    graphics.Clear(Color::White);

    SolidBrush brush(Color::Black);

    // All coordinates are now in the range [0.0f, 1.0f]
    graphics.ScaleTransform(scale, scale);

    // Center the drawing
    graphics.TranslateTransform(trX / scale, trY / scale);

    graphics.FillEllipse(&brush, 0.0f, 0.0f, 1.0f, 1.0f);

    EndPaint(hwnd, &ps);
}

void OnMazeResize(HWND hwnd, int newWidth, int newHeight)
{
    MazeControlExtra* mzCtrl = GetMazeControl(hwnd);

    mzCtrl->width = newWidth;
    mzCtrl->height = newHeight;
}
