#pragma once

#include <windows.h>


void OnResize(HWND hwnd, int resizeType, int newWidth, int newHeight);
void OnCreate(HWND hwnd, HINSTANCE hInstance);
void OnPaint(HWND hwnd);
void OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam);
void OnMenu(HWND hwnd, int idMenu);
void OnControl(HWND hwnd, HWND hwndControl, int idNotify, int idControl);
void OnGenerateClick(HWND hwnd, HWND hwndControl);
void OnVisualizeCheck(HWND hwnd, HWND hwndControl);
void CreateMenus(HWND hwnd);
void OnComboboxSelect(HWND hwnd, HWND hwndControl);
void OnNotify(HWND hwnd, NMHDR* msgStruct);
void OnExitSizeMove(HWND hwnd);
void OnFileSaveAs(HWND hwnd);
void OnFileOpen(HWND hwnd);
