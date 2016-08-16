#pragma once

#ifndef CELL_H
#define	CELL_H

enum CellState { Close, Open };

class Cell
{
public:
	int x;
	int y;
	CellState Left;
	CellState Right;
	CellState Top;
	CellState Bottom;
	bool Visited;
};

#endif	/* CELL_H */