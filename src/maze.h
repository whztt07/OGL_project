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

	void Transpose();

	void Draw2D(int argc, char** argv);

	virtual void MakeLabyrinth()
	{
	}

protected:

	Cell** labyrinth;
	int size;

	void Clear();
};

#endif	/* MAZE_H */