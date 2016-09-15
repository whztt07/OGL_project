#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>

#include "maze.h"

Maze::Maze(int size)
{
	Maze::size = size;

	for (int i = 0; i < size; i++) {
		vector<Cell> v_cell(size);
		for (int j = 0; j < size; j++) {
			v_cell[j].x = i;
			v_cell[j].y = j;
			v_cell[j].Visited = false;
			v_cell[j].Top = Close;
			v_cell[j].Bottom = Close;
			v_cell[j].Left = Close;
			v_cell[j].Right = Close;
		}
		labyrinth.push_back(v_cell);
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