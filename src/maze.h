#pragma once

#ifndef MAZE_H
#define	MAZE_H

#include <vector>

#include "cell.h"
#include "ogl_draw.h"

using namespace std;

class Maze
{
public:

	Maze(int width, int height);

	virtual void MakeLabyrinth()
	{
	}

	void Draw2D(int argc, char** argv);

protected:

	vector<vector<Cell>> labyrinth;
	int height;
	int width;
};

#endif	/* MAZE_H */