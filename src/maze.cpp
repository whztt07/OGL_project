#include "maze.h"

Maze::Maze(int width, int height)
{
	Maze::width = width;
	Maze::height = height;
	labyrinth.resize(height);
	for (int i = 0; i < height; i++) {
		labyrinth[i].resize(width);
		for (int j = 0; j < width; j++) {
			labyrinth[i][j].x = i;
			labyrinth[i][j].y = j;
			labyrinth[i][j].Visited = false;
		}
	}
}