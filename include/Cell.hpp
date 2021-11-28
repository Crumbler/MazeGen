#pragma once


constexpr char WallTop = 1,
               WallBottom = 2,
               WallRight = 4,
               WallLeft = 8,
               WallAll = WallTop | WallBottom | WallRight | WallLeft;

struct Cell
{
    int i, j;

    Cell(int i, int j) : i(i), j(j) { }
    Cell() {}
};
