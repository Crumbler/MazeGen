#include "Maze.hpp"
#include "Cell.hpp"

#include <cstdlib>
#include <cstdio>

#include <stack>

void Maze::genRecursiveBacktrack()
{
    constexpr int unvisited = WallAll;

    std::stack<Cell> cells;

    cells.push(Cell(rand() % size, rand() % size));

    while (!cells.empty())
    {
        Cell c = cells.top();

        Cells neighbors = getUnvisitedNeighbors(c.i, c.j);

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
