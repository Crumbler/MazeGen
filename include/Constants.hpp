#pragma once

constexpr int idButton = 0,
              idCheckbox = 1,
              idCombobox = 2,
              idEdit = 3,
              idUpdown = 4,
              idLabel1 = 5,
              idLabel2 = 6,
              idMazecontrol = 7;

constexpr int IDM_FILE_OPEN = 0,
              IDM_FILE_SAVE_AS = 1,
              IDM_EXIT = 2;

constexpr int mazeSizeMax = 20,
              mazeSizeMin = 2;

constexpr int totalAlgorithms = 2;

constexpr wchar_t AlgStrings[totalAlgorithms][20] = { L"Recursive backtrack", L"Binary tree" };

constexpr int panelWidth = 180,
              paddingRight = 20,
              paddingLeft = 10;
