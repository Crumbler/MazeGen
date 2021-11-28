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
    void SaveAs();
    void Load();
    int getSize();
    const char* getGrid();
    int ind(int i, int j);

private:
    int size;
    char* grid;

    void genBinaryTree();
    void genRecursiveBacktrack();

    void linkCells(int i1, int j1, int i2, int j2);
    void linkCells(Cell c1, Cell c2);

    Cells getUnvisitedNeighbors(int i, int j);
};
