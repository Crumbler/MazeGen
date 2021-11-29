#include "windowFuncs.hpp"
#include "Constants.hpp"
#include "mazeControl.hpp"
#include "Maze.hpp"

#include <CommCtrl.h>
#include <gdiplus.h>
#include <cstdio>
#include <cwchar>

using namespace Gdiplus;

RECT rectClient;
char currDir[256];
bool visualizing;
int mazeSize;
Alg currentAlgorithm;
HWND hwndCheckbox, hwndMazecontrol, hwndCombobox,
     hwndUpdown, hwndEdit;

void CreateMenus(HWND hwnd)
{
    HMENU hMenu = CreateMenu();
    HMENU hMenuPopup = CreateMenu();

    AppendMenuW(hMenuPopup, MF_STRING, IDM_FILE_OPEN, L"Open");
    AppendMenuW(hMenuPopup, MF_STRING, IDM_FILE_SAVE_AS, L"Save as...");
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hMenuPopup, MF_STRING, IDM_EXIT, L"Exit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, L"File");

    SetMenu(hwnd, hMenu);
}

void OnCreate(HWND hwnd, HINSTANCE hInstance)
{
    visualizing = false;
    currentAlgorithm = RecursiveBacktrack;
    mazeSize = initialMazeSize;
    GetCurrentDirectoryA(sizeof(currDir), currDir);

    CreateMenus(hwnd);

    CreateWindow(L"BUTTON", L"Generate",
                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                 paddingLeft, 10, panelWidth - paddingRight, 30, hwnd,
                  (HMENU)idButton, hInstance, nullptr);

    CreateWindowW(L"STATIC", L"Algorithm:",
                  WS_CHILD | WS_VISIBLE | SS_LEFT,
                  paddingLeft, 50, panelWidth - paddingRight, 20, hwnd,
                  (HMENU)idLabel1, hInstance, nullptr);

    hwndCombobox = CreateWindow(L"COMBOBOX", nullptr,
                                (WS_CHILD | WS_VISIBLE | CBS_DROPDOWN & ~CBS_SORT),
                                paddingLeft, 70, panelWidth - paddingRight, 300, hwnd,
                                (HMENU)idCombobox, hInstance, nullptr);

    for (int i = 0; i < totalAlgorithms; ++i)
    {
        SendMessage(hwndCombobox, CB_ADDSTRING, i,(LPARAM)AlgStrings[i]);
    }

    SendMessage(hwndCombobox, CB_SETCURSEL, currentAlgorithm, 0);

    CreateWindowW(L"STATIC", L"Maze size:",
                  WS_CHILD | WS_VISIBLE | SS_LEFT,
                  paddingLeft, 100, panelWidth - paddingRight, 20, hwnd,
                  (HMENU)idLabel2, hInstance, nullptr);

    hwndEdit = CreateWindowW(L"EDIT", nullptr,
                             WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
                             85, 100, 40, 20, hwnd,
                             (HMENU)idEdit, hInstance, nullptr);


    hwndUpdown = CreateWindowW(UPDOWN_CLASS, nullptr,
                               WS_CHILD | WS_VISIBLE | UDS_AUTOBUDDY |
                               UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_HOTTRACK,
                               0, 0, 0, 0, hwnd,
                               (HMENU)idUpdown, hInstance, nullptr);

    SendMessage(hwndUpdown, UDM_SETRANGE, 0, MAKELPARAM(mazeSizeMax, mazeSizeMin));
    SendMessage(hwndUpdown, UDM_SETPOS, 0, mazeSize);

    hwndCheckbox = CreateWindow(L"BUTTON", L"Visualize distance",
                                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                paddingLeft, 130, panelWidth - paddingRight, 20, hwnd,
                                (HMENU)idCheckbox, hInstance, nullptr);

    visualizing = SendMessage(hwndCheckbox, BM_GETCHECK, 0, 0);

    RegisterMazeControl(hInstance);

    hwndMazecontrol = CreateWindow(szMazeControl, nullptr,
                                   WS_CHILD | WS_VISIBLE & ~(CS_VREDRAW | CS_HREDRAW),
                                   panelWidth, 0, rectClient.right - panelWidth, rectClient.bottom, hwnd,
                                   (HMENU)idMazecontrol, hInstance, nullptr);
}

void OnResize(HWND hwnd, int resizeType, int newWidth, int newHeight)
{
    static int lastResizeType = 0;

    rectClient.right = newWidth;
    rectClient.bottom = newHeight;

    SetWindowPos(hwndMazecontrol, nullptr, 0, 0, newWidth - panelWidth, newHeight,
                 SWP_NOMOVE | SWP_NOREDRAW);

    // if maximized or unminimized
    if (resizeType == SIZE_MAXIMIZED ||
        (resizeType == 0 && lastResizeType != 0))
    {
        InvalidateRect(hwndMazecontrol, nullptr, false);
    }

    lastResizeType = resizeType;
}

void OnExitSizeMove(HWND hwnd)
{
    InvalidateRect(hwndMazecontrol, nullptr, false);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);

    Graphics graphics(hdc);

    graphics.Clear(Color::White);

    SolidBrush brush(Color(240, 240, 240));

    graphics.FillRectangle(&brush, 0, 0, panelWidth, rectClient.bottom);

    EndPaint(hwnd, &ps);
}

void OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if (lParam != 0)
    {
        OnControl(hwnd, (HWND)lParam, HIWORD(wParam), LOWORD(wParam));
    }
    else
    {
        OnMenu(hwnd, LOWORD(wParam));
    }
}

void OnMenu(HWND hwnd, int idMenu)
{
    switch (idMenu)
    {
    case IDM_FILE_OPEN:
        OnFileOpen(hwnd);
        break;

    case IDM_FILE_SAVE_AS:
        OnFileSaveAs(hwnd);
        break;

    case IDM_EXIT:
        SendMessage(hwnd, WM_CLOSE, 0, 0);
        break;
    }
}

void OnControl(HWND hwnd, HWND hwndControl, int idNotify, int idControl)
{
    switch (idControl)
    {
    case idButton:
        switch (idNotify)
        {
        case BN_CLICKED:
            OnGenerateClick(hwnd, hwndControl);
            break;
        }
        break;

    case idCheckbox:
        switch (idNotify)
        {
        case BN_CLICKED:
            OnVisualizeCheck(hwnd, hwndControl);
            break;
        }
        break;

    case idCombobox:
        switch (idNotify)
        {
        case LBN_SELCHANGE:
            OnComboboxSelect(hwnd, hwndControl);
            break;
        }
        break;
    }
}

void OnGenerateClick(HWND hwnd, HWND hwndControl)
{
    wchar_t str[3];

    GetWindowText(hwndEdit, str, 3);

    swscanf(str, L"%d", &mazeSize);

    SendMessage(hwndMazecontrol, WM_GENERATE, currentAlgorithm, mazeSize);
}

void OnVisualizeCheck(HWND hwnd, HWND hwndControl)
{
    visualizing = SendMessage(hwndControl, BM_GETCHECK, 0, 0);

    SendMessage(hwndMazecontrol, WM_SETVIS, visualizing, 0);
}

void OnComboboxSelect(HWND hwnd, HWND hwndControl)
{
    currentAlgorithm = (Alg)SendMessage(hwndControl, CB_GETCURSEL, 0, 0);
}

void OnNotify(HWND hwnd, NMHDR* msgStruct)
{
    int nCode = msgStruct->code;

    switch (nCode)
    {
    case UDN_DELTAPOS:
        wchar_t str[3];

        GetWindowText(hwndEdit, str, 3);

        swscanf(str, L"%d", &mazeSize);

        break;
    }
}

void OnFileSaveAs(HWND hwnd)
{
    OPENFILENAMEA ofn;
    char szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Maze (*.mz)\0*.MZ\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.lpstrDefExt = "mz";
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = currDir;
    ofn.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST |
                OFN_EXTENSIONDIFFERENT | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameA(&ofn))
    {
        SendMessage(hwndMazecontrol, WM_SAVEAS, 0, (LPARAM)ofn.lpstrFile);
    }
}

void OnFileOpen(HWND hwnd)
{
    OPENFILENAMEA ofn;
    char szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Maze (*.mz)\0*.MZ\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrDefExt = "mz";
    ofn.lpstrInitialDir = currDir;
    ofn.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST |
                OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn))
    {
        int res = SendMessage(hwndMazecontrol, WM_LOAD, 0, (LPARAM)ofn.lpstrFile);
        int alg = res >> 24;
        currentAlgorithm = (Alg)alg;

        SendMessage(hwndCombobox, CB_SETCURSEL, currentAlgorithm, 0);

        int size = res & 0x00FFFFFF;
        mazeSize = size;

        SendMessage(hwndUpdown, UDM_SETPOS, 0, mazeSize);
    }
}
