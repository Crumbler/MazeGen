#include "Maze.hpp"

#include <unordered_set>
#include <cstdlib>

void Maze::genPrim()
{
    std::unordered_set<Cell, Cell::HashFunction> cells;

    cells.insert(Cell(rand() % size, rand() % size));

    while (!cells.empty())
    {
        int ind = rand() % cells.size();
        auto cellIterator = cells.begin();
        for (int i = 0; i < ind; ++i)
        {
            ++cellIterator;
        }

        Cell c = *cellIterator;

        Cells neighbors = getUnlinkedNeighbors(c.i, c.j);

        if (neighbors.size != 0)
        {
            Cell c2 = neighbors.random();

            linkCells(c, c2);

            cells.insert(c2);
        }
        else
        {
            cells.erase(cellIterator);
        }
    }
}
