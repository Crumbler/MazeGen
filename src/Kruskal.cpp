#include "Maze.hpp"

#include <stack>
#include <vector>
#include <algorithm>
#include <utility>

using std::vector;
using std::pair;

void Maze::mergeSets(short* sets, Cell start,
                     short winnerSet, short loserSet)
{
    std::stack<Cell> path;

    path.push(start);

    while (!path.empty())
    {
        Cell curr = path.top();
        path.pop();

        sets[ind(curr.i, curr.j)] = winnerSet;

        Cells cells = getNeighbors(curr.i, curr.j);

        for (int i = 0; i < cells.size; ++i)
        {
            Cell c = cells[i];

            if (sets[ind(c.i, c.j)] == loserSet)
            {
                path.push(c);
            }
        }
    }
}

// get two neighbors belonging to different sets
void getTwoNeighbors(vector<pair<Cell, Cell>>& connections, short* sets,
                     int size, Cell& n1, Cell& n2)
{
    pair<Cell, Cell> cn;
    bool sameSet;
    Cell c1, c2;

    do
    {
        cn = connections[connections.size() - 1];
        connections.pop_back();

        c1 = cn.first,
        c2 = cn.second;

        sameSet = sets[c1.i * size + c1.j] == sets[c2.i * size + c2.j];
    }
    while (sameSet);

    n1 = c1;
    n2 = c2;
}

void initConnections(vector<pair<Cell, Cell>>& connections, int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = 0; j < size - 1; ++j)
        {
            connections.push_back(pair(Cell(i, j), Cell(i, j + 1)));
            connections.push_back(pair(Cell(i, j), Cell(i + 1, j)));
        }

        connections.push_back(pair(Cell(i, size - 1), Cell(i + 1, size - 1)));
    }

    for (int j = 0; j < size - 1; ++j)
    {
        connections.push_back(pair(Cell(size - 1, j), Cell(size - 1, j + 1)));
    }

    std::random_shuffle(connections.begin(), connections.end());
}

void Maze::genKruskal()
{
    short* sets = new short[size * size];

    for (int i = 0; i < size * size; ++i)
    {
        sets[i] = i;
    }

    vector<pair<Cell, Cell>> connections;
    initConnections(connections, size);

    int totalSets = size * size;

    while (totalSets > 1)
    {
        Cell c1, c2;
        getTwoNeighbors(connections, sets, size, c1, c2);

        linkCells(c1, c2);

        short set1 = sets[ind(c1.i, c1.j)],
              set2 = sets[ind(c2.i, c2.j)];

        --totalSets;
        mergeSets(sets, c2, set1, set2);
    }

    delete[] sets;
}
