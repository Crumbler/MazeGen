#include "windowFuncs.hpp"
#include "menuConstants.hpp"
#include "controlConstants.hpp"

#include <cstdio>

using namespace Gdiplus;

RECT rectClient;
Font* fontMain;
StringFormat* strfrmMain;
bool visualize;
HWND hwndCheckbox;
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

    CreateMenus(hwnd);

    HWND hwndButton = CreateWindow(L"BUTTON", L"Generate",
                                   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                   10, 10, 100, 30, hwnd,
                                   (HMENU)idButton, hInstance, nullptr);

    hwndCheckbox = CreateWindow(L"BUTTON", L"Visualize",
                                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                10, 130, 100, 30, hwnd,
                                (HMENU)idCheckbox, hInstance, nullptr);

    visualize = SendMessage(hwndCheckbox, BM_GETCHECK, 0, 0);

    HWND hwndCombobox = CreateWindow(L"COMBOBOX", nullptr,
                                     WS_CHILD | WS_VISIBLE | CBS_DROPDOWN & ~CBS_SORT,
                                     10, 50, 100, 300, hwnd,
                                     (HMENU)idCombobox, hInstance, nullptr);

    for (int i = 0; i < 2; ++i)
    {
        SendMessage(hwndCombobox, CB_ADDSTRING, i,(LPARAM)AlgStrings[i]);
    }

    SendMessage(hwndCombobox, CB_SETCURSEL, 0, 0);
}

void OnResize(HWND hwnd, int newWidth, int newHeight)
{
    rectClient.right = newWidth;
    rectClient.bottom = newHeight;

}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);

    Graphics graphics(hdc);

    graphics.Clear(Color::White);

    SolidBrush brush(Color::Beige);

    graphics.FillRectangle(&brush, 200, 200, 400, 400);

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
    }
}

void OnGenerateClick(HWND hwnd, HWND hwndControl)
{
    printf("Clicked on Generate\n");
}

void OnVisualizeCheck(HWND hwnd, HWND hwndControl)
{
    visualize = SendMessage(hwndControl, BM_GETCHECK, 0, 0);

    printf("check: %d\n", visualize);
}
