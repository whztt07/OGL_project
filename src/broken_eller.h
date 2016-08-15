#pragma once

#ifndef BROKEN_ELLER_H
#define	BROKEN_ELLER_H

#define N 20

using namespace std;

enum CellState { Close, Open };

class Cell
{
public:

	CellState left;
	CellState right;
	CellState up;
	CellState down;
};

class BrokenEller
{
public:

	BrokenEller();

	void lab();
	void faway();
	void setwave(short wave[][N], short i, short j, short value, short finish);
	
};

#endif	/* BROKEN_ELLER_H */