#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stack>
#include <vector>

#include "prim.h"

void Prim::MakeLabyrinth() {
	int width = size;
	int height = size;

	srand(time(NULL));
	int startX = rand() % width;
	int startY = rand() % height;

	labyrinth[startY][startX].Visited = true;

	vector<Cell> path;
	path.push_back(labyrinth[startX][startY]);

	while (!path.empty())
	{
		int curr_cell_num = rand() % path.size();

		Cell _cell = path[curr_cell_num];

		vector<Cell> nextStep;
		if (_cell.y > 0 && (labyrinth[_cell.y - 1][_cell.x].Visited == false))
			nextStep.push_back(labyrinth[_cell.y - 1][_cell.x]);
		if (_cell.y < height - 1 && (labyrinth[_cell.y + 1][_cell.x].Visited == false))
			nextStep.push_back(labyrinth[_cell.y + 1][_cell.x]);
		if (_cell.x > 0 && (labyrinth[_cell.y][_cell.x - 1].Visited == false))
			nextStep.push_back(labyrinth[_cell.y][_cell.x - 1]);
		if (_cell.x < width - 1 && (labyrinth[_cell.y][_cell.x + 1].Visited == false))
			nextStep.push_back(labyrinth[_cell.y][_cell.x + 1]);

		if (!nextStep.empty())
		{
			Cell next = nextStep[rand() % nextStep.size()];

			if (next.y != _cell.y)
			{
				if (_cell.y - next.y > 0)
				{
					labyrinth[_cell.y][_cell.x].Top = Open;
					labyrinth[next.y][next.x].Bottom = Open;
				}
				else
				{
					labyrinth[_cell.y][_cell.x].Bottom = Open;
					labyrinth[next.y][next.x].Top = Open;
				}
			}
			if (next.x != _cell.x)
			{
				if (_cell.x - next.x > 0)
				{
					labyrinth[_cell.y][_cell.x].Left = Open;
					labyrinth[next.y][next.x].Right = Open;
				}
				else
				{
					labyrinth[_cell.y][_cell.x].Right = Open;
					labyrinth[next.y][next.x].Left = Open;
				}
			}

			labyrinth[next.y][next.x].Visited = true;
			path.push_back(next);
		}
		else
		{
			path.erase(path.begin() + curr_cell_num);
		}
	}
}