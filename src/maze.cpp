#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>

#include "maze.h"

Maze::Maze(int size)
{
	Maze::size = size;
	labyrinth.resize(size);
	for (int i = 0; i < size; i++) {
		labyrinth[i].resize(size);
		for (int j = 0; j < size; j++) {
			labyrinth[i][j].x = i;
			labyrinth[i][j].y = j;
			labyrinth[i][j].Visited = false;
		}
	}
}

void Maze::Draw2D(int argc, char** argv)
{
	Draw(argc, argv, size);
}