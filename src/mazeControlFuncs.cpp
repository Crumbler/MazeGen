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
}

void PaintMaze(Graphics& graphics, const char grid[], int size)
{
    float cellSize = 1.0f / size,
          lineWidth = cellSize * wallWidth,
          halfLineWidth = lineWidth / 2.0f;

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

    Graphics graphics(hdc);

    graphics.Clear(Color::White);

    // All coordinates are now in the range [0.0f, 1.0f]
    graphics.ScaleTransform(scale, scale);

    // Center the drawing
    graphics.TranslateTransform(trX / scale, trY / scale);

    Maze& maze = mzCtrl->maze;

    int size = maze.getSize();

    PaintMaze(graphics, maze.getGrid(), size);

    EndPaint(hwnd, &ps);
}

void OnMazeResize(HWND hwnd, int newWidth, int newHeight)
{
    auto* mzCtrl = GetMazeControl(hwnd);

    mzCtrl->width = newWidth;
    mzCtrl->height = newHeight;
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
