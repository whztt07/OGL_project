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

	Maze(int size);

	~Maze();

	virtual void MakeLabyrinth()
	{
	}

	void Transpose();

	void Draw2D(int argc, char** argv);

protected:

	Cell** labyrinth;
	int size;
};

#endif	/* MAZE_H */