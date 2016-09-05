#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stack>
#include <vector>

#include "growing_tree.h"

void GrowingTree::MakeLabyrinth() {
	int width = size;
	int height = size;

	srand(time(NULL));
	int startX = rand() % width;
	int startY = rand() % height;

	labyrinth[startX][startY].Visited = true;

	vector<Cell> path;
	path.push_back(labyrinth[startX][startY]);

	while (!path.empty())
	{
		int curr_cell_num;
		if (rand() % 2)
			curr_cell_num = rand() % path.size();
		else
			curr_cell_num = path.size() - 1;

		Cell _cell = path[curr_cell_num];

		vector<Cell> nextStep;
		if (_cell.x > 0 && (labyrinth[_cell.x - 1][_cell.y].Visited == false))
			nextStep.push_back(labyrinth[_cell.x - 1][_cell.y]);
		if (_cell.x < width - 1 && (labyrinth[_cell.x + 1][_cell.y].Visited == false))
			nextStep.push_back(labyrinth[_cell.x + 1][_cell.y]);
		if (_cell.y > 0 && (labyrinth[_cell.x][_cell.y - 1].Visited == false))
			nextStep.push_back(labyrinth[_cell.x][_cell.y - 1]);
		if (_cell.y < height - 1 && (labyrinth[_cell.x][_cell.y + 1].Visited == false))
			nextStep.push_back(labyrinth[_cell.x][_cell.y + 1]);

		if (!nextStep.empty())
		{
			Cell next = nextStep[rand() % nextStep.size()];

			if (next.x != _cell.x)
			{
				if (_cell.x - next.x > 0)
				{
					labyrinth[_cell.x][_cell.y].Left = Open;
					labyrinth[next.x][next.y].Right = Open;
				}
				else
				{
					labyrinth[_cell.x][_cell.y].Right = Open;
					labyrinth[next.x][next.y].Left = Open;
				}
			}
			if (next.y != _cell.y)
			{
				if (_cell.y - next.y > 0)
				{
					labyrinth[_cell.x][_cell.y].Top = Open;
					labyrinth[next.x][next.y].Bottom = Open;
				}
				else
				{
					labyrinth[_cell.x][_cell.y].Bottom = Open;
					labyrinth[next.x][next.y].Top = Open;
				}
			}

			labyrinth[next.x][next.y].Visited = true;
			path.push_back(next);
		}
		else
		{
			path.erase(path.begin()+curr_cell_num);
		}
	}
}