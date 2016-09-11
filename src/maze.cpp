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
			labyrinth[i][j].Top = Close;
			labyrinth[i][j].Bottom = Close;
			labyrinth[i][j].Left = Close;
			labyrinth[i][j].Right = Close;
		}
	}
}

void Maze::Draw2D(int argc, char** argv)
{
	int msize = size*size;
	vector<Cell> maze(msize);

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			maze.push_back(labyrinth[i][j]);

	Draw(argc, argv, maze, size);
}