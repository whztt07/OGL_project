#pragma once

#ifndef MAZE_H
#define	MAZE_H

#include <vector>

#include "cell.h"
#include "ogl_draw_2D.h"
#include "ogl_draw_3D.h"

using namespace std;

class Maze
{
public:

	Maze();

	Maze(int _size);

	~Maze();

	void Transpose();

	void Draw(int argc, char** argv);

	virtual void MakeLabyrinth()
	{
	}

protected:

	Cell** labyrinth;
	int size;

	void Clear();
};

#endif	/* MAZE_H */
