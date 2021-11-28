#include "Maze.hpp"

#include <cstdlib>

void Maze::genBinaryTree()
{
    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = 0; j < size - 1; ++j)
        {
            int choice = rand() % 2;

            int i2 = i + choice,
                j2 = j + (1 - choice);

            linkCells(i, j, i2, j2);
        }

        linkCells(i, size - 1, i + 1, size - 1);
    }

    for (int j = 0; j < size - 1; ++j)
    {
        linkCells(size - 1, j, size - 1, j + 1);
    }
}
