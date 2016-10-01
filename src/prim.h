#pragma once

#ifndef PRIM_H
#define	PRIM_H

#include <vector>

#include "cell.h"
#include "maze.h"

class Prim : public Maze
{
public:

	Prim(int _size) : Maze(_size)
	{
	}

	void MakeLabyrinth();

};

#endif	/* PRIM_H */