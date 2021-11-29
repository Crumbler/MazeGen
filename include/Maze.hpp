#pragma once

#include "Cells.hpp"

enum Alg
{
    RecursiveBacktrack,
    BinaryTree
};

class Maze
{
public:
    Maze();
    ~Maze();
    void Generate(Alg alg, int size);
    void SaveAs(char* filename);
    int Load(char* filename);
    int getSize();
    const char* getGrid();
    const char* getColorGrid();
    int ind(int i, int j);

private:
    int size;
    Alg currAlg;
    char *grid, *colorGrid;
    short* distances;

    void calcDistances(short& maxDistance);
    void calcColors(short maxDistance);

    void genBinaryTree();
    void genRecursiveBacktrack();

    void linkCells(int i1, int j1, int i2, int j2);
    void linkCells(Cell c1, Cell c2);

    Cells getUnvisitedNeighbors(int i, int j);
    Cells getLinkedNeighbors(int i, int j);
    Cells getUncalculatedLinkedNeighbors(int i, int j);
};
