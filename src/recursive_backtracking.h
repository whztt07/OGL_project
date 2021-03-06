#pragma once

#ifndef RECURSIVE_BACKTRACKING_H
#define	RECURSIVE_BACKTRACKING_H

#include <vector>

#include "cell.h"
#include "maze.h"

class RecursiveBacktracking : public Maze
{
public:

	RecursiveBacktracking(int _size) : Maze(_size)
	{
	}

	void MakeLabyrinth();

};

#endif	/* RECURSIVE_BACKTRACKING_H */