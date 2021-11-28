#pragma once

#include "Cell.hpp"

struct Cells
{
    Cells();

    void Add(Cell c);
    void Add(int i, int j);

    Cell random();

    Cell& operator[](int index);

    Cell data[4];
    int size;

};
