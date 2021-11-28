#include "Maze.hpp"
#include "Cell.hpp"

#include <cstring>
#include <cstdio>

Maze::Maze()
{
    size = 2;
    grid = new char[size * size];

    grid[ind(0, 0)] = WallLeft | WallTop | WallRight;
    grid[ind(0, 1)] = WallLeft | WallTop | WallRight;
    grid[ind(1, 0)] = WallLeft | WallBottom;
    grid[ind(1, 1)] = WallRight | WallBottom;
}

Maze::~Maze()
{
    delete[] grid;
}

void Maze::Generate(Alg alg, int size)
{
    delete[] grid;
    this->size = size;

    grid = new char[size * size];

    memset(grid, WallAll, size * size);

    switch (alg)
    {
    case RecursiveBacktrack:
        genRecursiveBacktrack();
        break;

    case BinaryTree:
        genBinaryTree();
        break;
    }
}

void Maze::SaveAs()
{

}

void Maze::Load()
{

}

int Maze::getSize()
{
    return size;
}

const char* Maze::getGrid()
{
    return grid;
}

int Maze::ind(int i, int j)
{
    return i * size + j;
}

void Maze::linkCells(int i1, int j1, int i2, int j2)
{
    if (i1 < i2)
    {
        grid[ind(i1, j1)] &= ~WallBottom;
        grid[ind(i2, j2)] &= ~WallTop;
    }
    else if (i2 < i1)
    {
        grid[ind(i2, j2)] &= ~WallBottom;
        grid[ind(i1, j1)] &= ~WallTop;
    }
    else if (j1 < j2)
    {
        grid[ind(i1, j1)] &= ~WallRight;
        grid[ind(i2, j2)] &= ~WallLeft;
    }
    else
    {
        grid[ind(i2, j2)] &= ~WallRight;
        grid[ind(i1, j2)] &= ~WallLeft;
    }
}

void Maze::linkCells(Cell c1, Cell c2)
{
    linkCells(c1.i, c1.j, c2.i, c2.j);
}

Cells Maze::getUnvisitedNeighbors(int i, int j)
{
    Cells cells;

    if (i > 0 && grid[ind(i - 1, j)] == WallAll)
    {
        cells.Add(i - 1, j);
    }

    if (j > 0 && grid[ind(i, j - 1)] == WallAll)
    {
        cells.Add(i, j - 1);
    }

    if (j < size - 1 && grid[ind(i, j + 1)] == WallAll)
    {
        cells.Add(i, j + 1);
    }

    if (i < size - 1 && grid[ind(i + 1, j)] == WallAll)
    {
        cells.Add(i + 1, j);
    }

    return cells;
}
