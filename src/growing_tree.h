#pragma once

#ifndef GROWING_TREE_H
#define	GROWING_TREE_H

#include <vector>

#include "cell.h"

#define COMMON_CELL_COUNT 5

using namespace std;

class GrowingTree
{
public:

	GrowingTree(int width, int height);

	void MakeLabyrinth();

private:

	vector<vector<Cell>> labyrinth;
	int height;
	int width;
};

#endif	/* GROWING_TREE_H */