#include "Maze.hpp"

#include <unordered_set>
#include <vector>
#include <cstdlib>

Cell getRandomCell(std::unordered_set<Cell, Cell::HashFunction>& cells)
{
    int ind = rand() % cells.size();

    auto cellIterator = cells.begin();

    for (int i = 0; i < ind; ++i)
    {
        ++cellIterator;
    }

    return *cellIterator;
}

void Maze::genWilson()
{
    std::unordered_set<Cell, Cell::HashFunction> unvisited;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            unvisited.insert(Cell(i, j));
        }
    }

    Cell first = getRandomCell(unvisited);
    unvisited.erase(first);

    std::vector<Cell> path;

    while (!unvisited.empty())
    {
        path.clear();

        Cell c = getRandomCell(unvisited);

        path.push_back(c);

        while (unvisited.find(c) != unvisited.end())
        {
            c = getNeighbors(c.i, c.j).random();

            int ind = -1;
            for (int i = 0; i < path.size(); ++i)
            {
                if (path[i] == c)
                {
                    ind = i;
                    break;
                }
            }

            // if the cell was found in the path
            if (ind != -1)
            {
                path.erase(path.begin() + ind + 1, path.end());
            }
            else
            {
                path.push_back(c);
            }
        }

        for (int i = 0; i < path.size() - 1; ++i)
        {
            linkCells(path[i], path[i + 1]);
            unvisited.erase(path[i]);
        }

        linkCells(path[path.size() - 2], path[path.size() - 1]);
    }
}
