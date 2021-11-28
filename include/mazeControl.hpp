#pragma once

#include <windows.h>

#include "Maze.hpp"

constexpr wchar_t szMazeControl[] = L"MazeControl";

constexpr int WM_GENERATE = WM_USER + 1,
              WM_SETVIS = WM_GENERATE + 1,
              WM_SAVEAS = WM_SETVIS + 1,
              WM_LOAD = WM_SAVEAS + 1;

void RegisterMazeControl(HINSTANCE hInstance);

struct MazeControlExtra
{
    int width, height;
    bool visualizing;
    Maze maze;
};
