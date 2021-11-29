#include "Maze.hpp"
#include "Cell.hpp"

#include <cstdlib>

#include <stack>

void Maze::genRecursiveBacktrack()
{
    std::stack<Cell> cells;

    cells.push(Cell(rand() % size, rand() % size));

    while (!cells.empty())
    {
        Cell c = cells.top();

        Cells neighbors = getUnlinkedNeighbors(c.i, c.j);

        if (neighbors.size != 0)
        {
            Cell c2 = neighbors.random();

            linkCells(c, c2);

            cells.push(c2);
        }
        else
        {
            cells.pop();
        }
    }
}
