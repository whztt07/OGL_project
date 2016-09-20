#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stack>
#include <vector>

#include "recursive_backtracking.h"

void RecursiveBacktracking::MakeLabyrinth() {
	int width = size;
	int height = size;

	srand(time(NULL));
	int startX = rand() % width;
	int startY = rand() % height;

	labyrinth[startY][startX].Visited = true;

	stack<Cell> path;
	path.push(labyrinth[startY][startX]);

	while (!path.empty())
	{
		Cell _cell = path.top();

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
			path.push(next);
		}
		else
		{
			path.pop();
		}
	}
}