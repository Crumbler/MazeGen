#include "Maze.hpp"
#include "Cell.hpp"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <stack>

Maze::Maze()
{
    size = 10;
    grid = new char[size * size];
    colorGrid = new char[size * size];
    distances = new short[size * size];

    Generate(RecursiveBacktrack, size);
}

Maze::~Maze()
{
    delete[] grid;
    delete[] colorGrid;
    delete[] distances;
}

void Maze::Generate(Alg alg, int size)
{
    if (this->size != size)
    {
        delete[] grid;
        delete[] colorGrid;
        delete[] distances;

        this->size = size;

        grid = new char[size * size];
        colorGrid = new char[size * size];
        distances = new short[size * size];
    }

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

    short maxDistance;

    calcDistances(maxDistance);
    calcColors(maxDistance);
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

const char* Maze::getColorGrid()
{
    return colorGrid;
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
        grid[ind(i1, j1)] &= ~WallLeft;
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

Cells Maze::getLinkedNeighbors(int i, int j)
{
    Cells cells;

    if (i > 0 && (grid[ind(i, j)] & WallTop) == 0)
    {
        cells.Add(i - 1, j);
    }

    if (j > 0 && (grid[ind(i, j)] & WallLeft) == 0)
    {
        cells.Add(i, j - 1);
    }

    if (j < size - 1 && (grid[ind(i, j)] & WallRight) == 0)
    {
        cells.Add(i, j + 1);
    }

    if (i < size - 1 && (grid[ind(i, j)] & WallBottom) == 0)
    {
        cells.Add(i + 1, j);
    }

    return cells;
}

Cells Maze::getUncalculatedLinkedNeighbors(int i, int j)
{
    Cells cells;

    if (i > 0 && distances[ind(i - 1, j)] == -1 && (grid[ind(i, j)] & WallTop) == 0)
    {
        cells.Add(i - 1, j);
    }

    if (j > 0 && distances[ind(i, j - 1)] == -1 && (grid[ind(i, j)] & WallLeft) == 0)
    {
        cells.Add(i, j - 1);
    }

    if (j < size - 1 && distances[ind(i, j + 1)] == -1 && (grid[ind(i, j)] & WallRight) == 0)
    {
        cells.Add(i, j + 1);
    }

    if (i < size - 1 && distances[ind(i + 1, j)] == -1 && (grid[ind(i, j)] & WallBottom) == 0)
    {
        cells.Add(i + 1, j);
    }

    return cells;
}

void Maze::calcDistances(short& maxDistance)
{
    std::stack<Cell> cells;

    short distance = 0, maxDist = 0;

    memset(distances, -1, sizeof(short) * size * size);

    cells.push(Cell(size / 2, size / 2));

    while (!cells.empty())
    {
        Cell c = cells.top();

        distances[ind(c.i, c.j)] = distance;
        maxDist = std::max(maxDist, distance);

        Cells neighbors = getUncalculatedLinkedNeighbors(c.i, c.j);

        if (neighbors.size != 0)
        {
            ++distance;

            cells.push(neighbors.random());
        }
        else
        {
            cells.pop();
            --distance;
        }
    }

    maxDistance = maxDist;
}

void Maze::calcColors(short maxDistance)
{
    float maxDist = maxDistance;

    for (int i = 0; i < size * size; ++i)
    {
        colorGrid[i] = 255 - (distances[i] / maxDist) * 255.0f;
    }
}
