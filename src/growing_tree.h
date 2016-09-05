#pragma once

#ifndef GROWING_TREE_H
#define	GROWING_TREE_H

#include <vector>

#include "cell.h"
#include "maze.h"

class GrowingTree : public Maze
{
public:

	GrowingTree(int size) : Maze(size) 
	{
	}

	void MakeLabyrinth();

};

#endif	/* GROWING_TREE_H */