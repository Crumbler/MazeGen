#pragma once

#include <vector>

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

    bool operator==(const Cell& c) const
    {
        return i == c.i && j == c.j;
    }

    struct HashFunction
    {
        size_t operator()(const Cell& c) const
        {
            size_t xHash = std::hash<int>()(c.i);
            size_t yHash = std::hash<int>()(c.j) << 1;
            return xHash ^ yHash;
        }
    };
};
