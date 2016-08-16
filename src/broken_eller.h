#pragma once

#ifndef BROKEN_ELLER_H
#define	BROKEN_ELLER_H

#define CELL_COUNT 20

using namespace std;

class BrokenEller
{
public:

	BrokenEller();

	void MakeLabirint();
	void FindTheWay();

private:

	void SetWaveValue(short wave[][CELL_COUNT], short i, short j, short value, short finish);
};

#endif	/* BROKEN_ELLER_H */