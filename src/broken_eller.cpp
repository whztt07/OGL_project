#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "broken_eller.h"

BrokenEller::BrokenEller()
{
}

void BrokenEller::MakeLabyrinth() {
	short i, j, set[CELL_COUNT][CELL_COUNT], uniq = 1, k = 0;
	srand(time(0));
	for (i = 0; i<CELL_COUNT; i++) {
		cell[i][CELL_COUNT - 1].Right = Open;
		cell[i][0].Left = Open;
	}
	for (i = 0; i<CELL_COUNT; i++) {
		set[0][i] = 0;
		cell[0][i].Top = Open;
	}
	while (k<CELL_COUNT) {
		for (i = 0; i<CELL_COUNT; i++)
			if (!set[k][i])
				set[k][i] = uniq++;
		for (i = 0; i<CELL_COUNT - 1; i++) {
			if (set[k][i] == set[k][i + 1])
				cell[k][i].Right = Open;
			else {
				if (rand() % 2)
					cell[k][i].Right = Open;
				else {
					cell[k][i].Right = Close;
					set[k][i + 1] = set[k][i];
				}
			}
		}
		for (i = 0; i<CELL_COUNT; i++) {
			if (rand() % 2)
				cell[k][i].Bottom = Open;
			else
				cell[k][i].Bottom = Close;
			if (set[k][i - 1] != set[k][i] && set[k][i] != set[k][i + 1])
				cell[k][i].Bottom = Close;
			else {
				if (set[k][i] != set[k][i + 1]) {
					j = i - 1;
					short flag = 1;
					while (set[k][j] == set[k][i]) {
						if (!cell[k][j].Bottom) {
							flag = 0;
							break;
						}
						j--;
					}
					if (flag)
						cell[k][i].Bottom = Close;
				}
			}
		}
		if (k<CELL_COUNT - 1) {
			k++;
			for (i = 0; i<CELL_COUNT; i++) {
				if (cell[k - 1][i].Bottom) {
					set[k][i] = 0;
					cell[k][i].Top = Open;
				}
				else {
					set[k][i] = set[k - 1][i];
					cell[k][i].Top = Close;
				}
				if (cell[k - 1][i].Right)
					cell[k - 1][i + 1].Left = Open;
				else
					cell[k - 1][i + 1].Left = Close;
				cell[k][i].Bottom = Close;
			}
			for (i = 0; i<CELL_COUNT - 1; i++)
				cell[k][i].Right = Close;
		}
		else {
			for (i = 0; i<CELL_COUNT; i++) {
				cell[k][i].Bottom = Open;
				if (set[k][i] != set[k][i + 1])
					cell[k][i].Right = Close;
				cell[k][i].Left = cell[k][i - 1].Right;
			}
			cell[k][CELL_COUNT - 1].Right = Open;
			k++;
		}
	}
	cell[0][rand() % CELL_COUNT].Top = Close;
	cell[CELL_COUNT - 1][rand() % CELL_COUNT].Bottom = Close;
}

void BrokenEller::FindTheWay() {
	short wave[CELL_COUNT][CELL_COUNT], i, j, start, fin, len;
	bool way[CELL_COUNT][CELL_COUNT];
	for (i = 0; i<CELL_COUNT; i++)
		if (!cell[0][i].Top) {
			wave[0][i] = CELL_COUNT*CELL_COUNT;
			start = i;
		}
		else
			wave[0][i] = CELL_COUNT*CELL_COUNT;
	for (i = 1; i<CELL_COUNT; i++)
		for (j = 0; j<CELL_COUNT; j++)
			wave[i][j] = CELL_COUNT*CELL_COUNT;
	for (i = 0; i<CELL_COUNT; i++)
		if (!cell[CELL_COUNT - 1][i].Bottom) {
			fin = i;
			break;
		}
	SetWaveValue(wave, 0, start, 0, fin);
	printf("Shortest way = %d\n", wave[CELL_COUNT - 1][fin]);
	for (i = 0; i<CELL_COUNT; i++)
		for (j = 0; j<CELL_COUNT; j++)
			way[i][j] = 0;
	way[CELL_COUNT - 1][fin] = 1;
	i = CELL_COUNT - 1;
	j = fin;
	len = wave[i][j];
	while (len) {
		if (wave[i - 1][j] == len - 1 && i>0 && !cell[i][j].Top) {
			len--;
			i--;
			way[i][j] = 1;
		}
		if (wave[i][j - 1] == len - 1 && j>0 && !cell[i][j].Left) {
			len--;
			j--;
			way[i][j] = 1;
		}
		if (wave[i][j + 1] == len - 1 && j<CELL_COUNT - 1 && !cell[i][j].Right) {
			len--;
			j++;
			way[i][j] = 1;
		}
		if (wave[i + 1][j] == len - 1 && i<CELL_COUNT - 1 && !cell[i][j].Bottom) {
			len--;
			i++;
			way[i][j] = 1;
		}
	}
	way[0][start] = 1;
}

void BrokenEller::SetWaveValue(short wave[][CELL_COUNT], short i, short j, short value, short finish) {
	if (wave[i][j]>value && value<wave[CELL_COUNT - 1][finish])
		wave[i][j] = value;
	else
		return;
	if (!cell[i][j].Bottom)
		if (i<CELL_COUNT - 1)
			SetWaveValue(wave, i + 1, j, value + 1, finish);
		else
			return;
	if (!cell[i][j].Top && i>0)
		SetWaveValue(wave, i - 1, j, value + 1, finish);
	if (!cell[i][j].Left && j>0)
		SetWaveValue(wave, i, j - 1, value + 1, finish);
	if (!cell[i][j].Right && j<CELL_COUNT - 1)
		SetWaveValue(wave, i, j + 1, value + 1, finish);
}