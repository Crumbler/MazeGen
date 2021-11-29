#include "Maze.hpp"

#include <cstdlib>

void Maze::genAldousBroder()
{
    // the starting cell is considered already visited
    int unvisitedCells = size * size - 1;

    Cell c(rand() % size, rand() % size);

    while (unvisitedCells > 0)
    {
        Cells unvisitedNbrs = getUnlinkedNeighbors(c.i, c.j);

        if (unvisitedNbrs.size != 0)
        {
            Cell c2 = unvisitedNbrs.random();

            linkCells(c, c2);

            c = c2;

            --unvisitedCells;
        }
        else
        {
            c = getNeighbors(c.i, c.j).random();
        }
    }
}
