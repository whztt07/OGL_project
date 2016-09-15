#pragma once

#ifndef CELL_H
#define	CELL_H

#define COMMON_CELL_COUNT 10

enum CellState { Close, Open };

class Cell
{
public:
	Cell()
	{
		//x = 99;
		//y = 99;
		Left = Close;
		Right = Close;
		Top = Close;
		Bottom = Close;
		Visited = false;
	}

	int x;
	int y;
	CellState Left;
	CellState Right;
	CellState Top;
	CellState Bottom;
	bool Visited;
};

#endif	/* CELL_H */