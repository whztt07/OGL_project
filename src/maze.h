#pragma once

#ifndef MAZE_H
#define	MAZE_H

#include <vector>

#include "cell.h"

using namespace std;

class Maze
{
public:

	Maze(int width, int height);

	virtual void MakeLabyrinth()
	{
	}

protected:

	vector<vector<Cell>> labyrinth;
	int height;
	int width;
};

#endif	/* MAZE_H */