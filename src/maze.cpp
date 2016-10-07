#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>

#include "maze.h"

Maze::Maze()
{
	size = COMMON_CELL_COUNT;

	labyrinth = new Cell*[size];

	for (int y = 0; y < size; y++) {
		labyrinth[y] = new Cell[size];
		for (int x = 0; x < size; x++) {
			labyrinth[y][x].x = x;
			labyrinth[y][x].y = y;
			labyrinth[y][x].Visited = false;
			labyrinth[y][x].Top = Close;
			labyrinth[y][x].Bottom = Close;
			labyrinth[y][x].Left = Close;
			labyrinth[y][x].Right = Close;
		}
	}
}

Maze::Maze(int _size)
{
	size = _size;

	labyrinth = new Cell*[size];

	for (int y = 0; y < size; y++) {
		labyrinth[y] = new Cell[size];
		for (int x = 0; x < size; x++) {
			labyrinth[y][x].x = x;
			labyrinth[y][x].y = y;
			labyrinth[y][x].Visited = false;
			labyrinth[y][x].Top = Close;
			labyrinth[y][x].Bottom = Close;
			labyrinth[y][x].Left = Close;
			labyrinth[y][x].Right = Close;
		}
	}
}

Maze::~Maze()
{
	Clear();
}

void Maze::Clear()
{
	for (int i = 0; i < size; i++) {
		delete[] labyrinth[i];
	}
	delete[] labyrinth;
}

void Maze::Transpose()
{
	Cell** _labyrinth = new Cell*[size];

	for (int y = 0; y < size; y++) {
		_labyrinth[y] = new Cell[size];
		for (int x = 0; x < size; x++) {
			_labyrinth[y][x] = labyrinth[x][y];
		}
	}
	
	Clear();

	labyrinth = _labyrinth;
}

void Maze::Draw2D(int argc, char** argv)
{
	int msize = size*size;
	vector<Cell> maze(msize);

	for (int y = 0, k = 0; y < size; y++)
		for (int x = 0; x < size; x++, k++)
			maze[k] = labyrinth[y][x];

	//Draw(argc, argv, maze, size);
}