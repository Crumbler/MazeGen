#pragma once

#include <windows.h>


void OnVScroll(HWND hwnd, WPARAM wParam);
void OnResize(HWND hwnd, int newWidth, int newHeight);
void OnCreate(HWND hwnd, HINSTANCE hInstance);
void OnPaint(HWND hwnd);
void OnMouseWheel(HWND hwnd, WPARAM scrollState);
void OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam);
void OnMenu(HWND hwnd, int idMenu);
void OnControl(HWND hwnd, HWND hwndControl, int idNotify, int idControl);
void OnGenerateClick(HWND hwnd, HWND hwndControl);
void OnVisualizeCheck(HWND hwnd, HWND hwndControl);
void CreateMenus(HWND hwnd);
void OnComboboxSelect(HWND hwnd, HWND hwndControl);
void OnNotify(HWND hwnd, NMHDR* msgStruct);
