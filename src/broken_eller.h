#pragma once

#ifndef BROKEN_ELLER_H
#define	BROKEN_ELLER_H

#include "cell.h"

#define CELL_COUNT 20

class BrokenEller
{
public:

	BrokenEller();

	void MakeLabyrinth();
	void FindTheWay();

private:

	Cell cell[CELL_COUNT][CELL_COUNT];

	void SetWaveValue(short wave[][CELL_COUNT], short i, short j, short value, short finish);
};

#endif	/* BROKEN_ELLER_H */