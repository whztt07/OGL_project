#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>

#include "maze.h"

Maze::Maze(int size)
{
	Maze::size = size+1;
	labyrinth.resize(size+1);
	for (int i = 0; i <= size; i++) {
		labyrinth[i].resize(size+1);
		for (int j = 0; j <= size; j++) {
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
	Cell* maze = new Cell[size*size];
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			maze[i*size + j].x = labyrinth[i][j].x;
			maze[i*size + j].y = labyrinth[i][j].y;
			maze[i*size + j].Visited = labyrinth[i][j].Visited;
			maze[i*size + j].Top = labyrinth[i][j].Top;
			maze[i*size + j].Bottom = labyrinth[i][j].Bottom;
			maze[i*size + j].Left = labyrinth[i][j].Left;
			maze[i*size + j].Right = labyrinth[i][j].Right;
		}
	}
	Draw(argc, argv, maze, size);
}