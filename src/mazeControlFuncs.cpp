#include "mazeControlFuncs.hpp"
#include "mazeControl.hpp"
#include "Cell.hpp"

#include <cstdio>

#include <gdiplus.h>

using namespace Gdiplus;

constexpr float wallWidth = 0.1f;

MazeControlExtra* GetMazeControl(HWND hwnd)
{
    return (MazeControlExtra*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

void OnMazeCreate(HWND hwnd)
{
    auto* mzctl = new MazeControlExtra();

    mzctl->visualizing = false;

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)mzctl);

    HDC hdc = GetDC(hwnd);

    mzctl->memDC = CreateCompatibleDC(hdc);

    RECT rectClient;
    GetClientRect(hwnd, &rectClient);

    mzctl->bmp = CreateCompatibleBitmap(mzctl->memDC, rectClient.right, rectClient.bottom);

    SelectObject(mzctl->memDC, mzctl->bmp);

    ReleaseDC(hwnd, hdc);
}

void OnMazeClose(HWND hwnd)
{
    auto* mzCtrl = GetMazeControl(hwnd);

    delete mzCtrl;
}

void PaintMaze(Graphics& graphics, const char grid[],
               const char colorGrid[], int size, bool visualize)
{
    float cellSize = 1.0f / size,
          lineWidth = cellSize * wallWidth,
          halfLineWidth = lineWidth / 2.0f;

    if (visualize)
    {
        Color color;
        SolidBrush brush(color);

        for (int i = 0; i < size; ++i)
        {
            float y = i * cellSize;

            for (int j = 0; j < size; ++j)
            {
                float x = j * cellSize;

                short val = colorGrid[i * size + j];
                color = Color(val, val, 255);
                brush.SetColor(color);
                graphics.FillRectangle(&brush, x, y,
                                       cellSize, cellSize);
            }
        }
    }

    Pen penWall(Color::Black, lineWidth);

    graphics.DrawLine(&penWall, 0.0f, 0.0f, 1.0f, 0.0f);
    graphics.DrawLine(&penWall, 0.0f, 0.0f, 0.0f, 1.0f);

    for (int i = 0; i < size; ++i)
    {
        float y = i * cellSize;

        for (int j = 0; j < size; ++j)
        {
            char cell = grid[i * size + j];
            float x = j * cellSize;

            if (cell & WallRight)
            {
                graphics.DrawLine(&penWall, x + cellSize, y - halfLineWidth,
                                  x + cellSize, y + cellSize + halfLineWidth);
            }

            if (cell & WallBottom)
            {
                graphics.DrawLine(&penWall, x - halfLineWidth, y + cellSize,
                                  x + cellSize + halfLineWidth, y + cellSize);
            }
        }
    }
}

void OnMazePaint(HWND hwnd)
{
    auto* mzCtrl = GetMazeControl(hwnd);

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

    Graphics graphics(mzCtrl->memDC);

    graphics.Clear(Color::White);

    // All coordinates are now in the range [0.0f, 1.0f]
    graphics.ScaleTransform(scale, scale);

    // Center the drawing
    graphics.TranslateTransform(trX / scale, trY / scale);

    Maze& maze = mzCtrl->maze;

    int size = maze.getSize();

    PaintMaze(graphics, maze.getGrid(), maze.getColorGrid(), size, mzCtrl->visualizing);

    BitBlt(hdc, 0, 0, width, height,
           mzCtrl->memDC, 0, 0, SRCCOPY);

    EndPaint(hwnd, &ps);
}

void OnMazeResize(HWND hwnd, int newWidth, int newHeight)
{
    auto* mzCtrl = GetMazeControl(hwnd);

    mzCtrl->width = newWidth;
    mzCtrl->height = newHeight;

    HDC hdc = GetDC(hwnd);

    mzCtrl->bmp = CreateCompatibleBitmap(hdc, mzCtrl->width, mzCtrl->height);

    auto oldBmp = SelectObject(mzCtrl->memDC, mzCtrl->bmp);
    DeleteObject(oldBmp);

    ReleaseDC(hwnd, hdc);
}

void OnMazeGenerate(HWND hwnd, Alg alg, int mazeSize)
{
    auto* mzlt = GetMazeControl(hwnd);

    mzlt->maze.Generate(alg, mazeSize);

    InvalidateRect(hwnd, nullptr, false);
}

void OnSetVisualizing(HWND hwnd, bool visualizing)
{
    auto* mzlt = GetMazeControl(hwnd);

    mzlt->visualizing = visualizing;

    InvalidateRect(hwnd, nullptr, false);
}

void OnMazeSaveAs(HWND hwnd, char* filename)
{
    auto* mzlt = GetMazeControl(hwnd);

    mzlt->maze.SaveAs(filename);
}

int OnMazeLoad(HWND hwnd, char* filename)
{
    auto* mzlt = GetMazeControl(hwnd);

    int res = mzlt->maze.Load(filename);

    if (res)
    {
        InvalidateRect(hwnd, nullptr, false);
    }

    return res;
}
