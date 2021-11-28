#pragma once

#include "Maze.hpp"
#include <windows.h>

void OnMazeCreate(HWND hwnd);
void OnMazeClose(HWND hwnd);
void OnMazePaint(HWND hwnd);
void OnMazeResize(HWND hwnd, int newWidth, int newHeight);
void OnMazeGenerate(HWND hwnd, Alg alg, int mazeSize);
void OnSetVisualizing(HWND hwnd, bool visualizing);
