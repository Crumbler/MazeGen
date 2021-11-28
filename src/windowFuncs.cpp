#include "windowFuncs.hpp"
#include "Constants.hpp"
#include "mazeControl.hpp"


#include <CommCtrl.h>
#include <gdiplus.h>
#include <cstdio>

using namespace Gdiplus;

RECT rectClient;
Font* fontMain;
StringFormat* strfrmMain;
bool visualize;
int currentAlgorithm, mazeSize;
HWND hwndCheckbox, hwndMazecontrol;
wchar_t AlgStrings[2][7] = { L"First", L"Second" };


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
    fontMain = new Font(L"Times New Roman", 25);
    strfrmMain = StringFormat::GenericDefault()->Clone();

    visualize = false;
    currentAlgorithm = 0;
    mazeSize = mazeSizeMin;

    CreateMenus(hwnd);

    CreateWindow(L"BUTTON", L"Generate",
                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                 10, 10, 115, 30, hwnd,
                  (HMENU)idButton, hInstance, nullptr);

    CreateWindowW(L"STATIC", L"Algorithm:",
                  WS_CHILD | WS_VISIBLE | SS_LEFT,
                  10, 50, 115, 20, hwnd,
                  (HMENU)idLabel1, hInstance, nullptr);

    HWND hwndCombobox = CreateWindow(L"COMBOBOX", nullptr,
                                     WS_CHILD | WS_VISIBLE | CBS_DROPDOWN & ~CBS_SORT,
                                     10, 70, 115, 300, hwnd,
                                     (HMENU)idCombobox, hInstance, nullptr);

    for (int i = 0; i < 2; ++i)
    {
        SendMessage(hwndCombobox, CB_ADDSTRING, i,(LPARAM)AlgStrings[i]);
    }

    SendMessage(hwndCombobox, CB_SETCURSEL, currentAlgorithm, 0);

    CreateWindowW(L"STATIC", L"Maze size:",
                  WS_CHILD | WS_VISIBLE | SS_LEFT,
                  10, 100, 115, 20, hwnd,
                  (HMENU)idLabel2, hInstance, nullptr);

    CreateWindowW(L"EDIT", nullptr,
                  WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
                  85, 100, 40, 20, hwnd,
                  (HMENU)idEdit, hInstance, nullptr);


    HWND hwndUpdown = CreateWindowW(UPDOWN_CLASS, nullptr,
                                    WS_CHILD | WS_VISIBLE | UDS_AUTOBUDDY |
                                    UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_HOTTRACK,
                                    0, 0, 0, 0, hwnd,
                                    (HMENU)idUpdown, hInstance, nullptr);

    SendMessage(hwndUpdown, UDM_SETRANGE, 0, MAKELPARAM(mazeSizeMax, mazeSizeMin));
    SendMessage(hwndUpdown, UDM_SETPOS, 0, mazeSize);

    hwndCheckbox = CreateWindow(L"BUTTON", L"Visualize distance",
                                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_MULTILINE,
                                10, 130, 115, 40, hwnd,
                                (HMENU)idCheckbox, hInstance, nullptr);

    visualize = SendMessage(hwndCheckbox, BM_GETCHECK, 0, 0);

    RegisterMazeControl(hInstance);

    hwndMazecontrol = CreateWindow(szMazeControl, nullptr,
                                   WS_CHILD | WS_VISIBLE,
                                   135, 0, rectClient.right - 135, rectClient.bottom, hwnd,
                                   (HMENU)idMazecontrol, hInstance, nullptr);
}

void OnResize(HWND hwnd, int newWidth, int newHeight)
{
    rectClient.right = newWidth;
    rectClient.bottom = newHeight;

    SetWindowPos(hwndMazecontrol, nullptr, 0, 0, newWidth - 135, newHeight, SWP_NOMOVE);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);

    Graphics graphics(hdc);

    graphics.Clear(Color::White);

    SolidBrush brush(Color(240, 240, 240));

    graphics.FillRectangle(&brush, 0, 0, 135, rectClient.bottom);

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
    printf("Received menu message: %d\n", idMenu);

    switch (idMenu)
    {
    case IDM_FILE_OPEN:

        break;

    case IDM_FILE_SAVE_AS:

        break;

    case IDM_EXIT:
        SendMessage(hwnd, WM_CLOSE, 0, 0);
        break;
    }
}

void OnControl(HWND hwnd, HWND hwndControl, int idNotify, int idControl)
{
    printf("Received notification %d from control %d\n", idNotify, idControl);

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
    printf("Clicked on Generate\n");

    SendMessage(hwndMazecontrol, WM_GENERATE, 0, 0);
}

void OnVisualizeCheck(HWND hwnd, HWND hwndControl)
{
    visualize = SendMessage(hwndControl, BM_GETCHECK, 0, 0);

    printf("check: %d\n", visualize);
}

void OnComboboxSelect(HWND hwnd, HWND hwndControl)
{
    currentAlgorithm = SendMessage(hwndControl, CB_GETCURSEL, 0, 0);

    printf("Selection changed: %d\n", currentAlgorithm);
}

void OnNotify(HWND hwnd, NMHDR* msgStruct)
{
    int nCode = msgStruct->code;

    switch (nCode)
    {
    case UDN_DELTAPOS:
        LPNMUPDOWN lpnmud = (LPNMUPDOWN)msgStruct;

        int newMazeSize = mazeSize + lpnmud->iDelta;

        if (newMazeSize >= mazeSizeMin && newMazeSize <= mazeSizeMax)
        {
            mazeSize = newMazeSize;
        }

        break;
    }
}
