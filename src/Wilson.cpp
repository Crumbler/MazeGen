#include "Maze.hpp"

#include <cstdlib>

void Maze::genWilson()
{
    std::vector<Cell> cells;

    for (int i = size - 1; i >= 0; --i)
    {
        int start = 0;

        for (int j = 0; j < size; ++j)
        {
            Cell c(i, j);

            bool atEast = j == size - 1,
                 atNorth = i == 0;

            if (atEast || (!atNorth && (rand() % 2)))
            {
                c = Cell(i, start + rand() % (j - start + 1));

                if (!atNorth)
                {
                    linkCells(c.i, c.j, c.i - 1, c.j);
                }

                start = j + 1;
            }
            else
            {
                linkCells(c.i, c.j, c.i, c.j + 1);
            }
        }
    }
}
