#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>

#include "maze.h"

Maze::Maze(int size)
{
	Maze::size = size;

	labyrinth = new Cell*[size];

	for (int i = 0; i < size; i++) {
		labyrinth[i] = new Cell[size];
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

Maze::~Maze()
{
	for (int i = 0; i < size; i++) {
		delete[] labyrinth[i];
	}
	delete[] labyrinth;
}

void Maze::Draw2D(int argc, char** argv)
{
	int msize = size*size;
	vector<Cell> maze(msize);

	for (int i = 0, k = 0; i < size; i++)
		for (int j = 0; j < size; j++, k++)
			maze[k] = labyrinth[i][j];

	Draw(argc, argv, maze, size);
}