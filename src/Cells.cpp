#include "Cells.hpp"

#include <cstdlib>

Cells::Cells()
{
    size = 0;
}

void Cells::Add(Cell c)
{
    data[size] = c;
    ++size;
}

void Cells::Add(int i, int j)
{
    data[size] = Cell(i, j);
    ++size;
}

Cell Cells::random()
{
    return data[rand() % size];
}

Cell& Cells::operator[](int index)
{
    return data[index];
}
