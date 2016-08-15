#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "broken_eller.h"

Cell cell[N][N];

BrokenEller::BrokenEller()
{
}

void BrokenEller::lab() {
	short i, j, mn[N][N], uniq = 1, k = 0;
	srand(time(0));
	for (i = 0; i<N; i++) {
		cell[i][N - 1].right = Open;
		cell[i][0].left = Open;
	}
	for (i = 0; i<N; i++) {
		mn[0][i] = 0;
		cell[0][i].up = Open;
	}
	while (k<N) {
		for (i = 0; i<N; i++)
			if (!mn[k][i])
				mn[k][i] = uniq++;
		for (i = 0; i<N - 1; i++) {
			if (mn[k][i] == mn[k][i + 1])
				cell[k][i].right = Open;
			else {
				if (rand() % 2)
					cell[k][i].right = Open;
				else {
					cell[k][i].right = Close;
					mn[k][i + 1] = mn[k][i];
				}
			}
		}
		for (i = 0; i<N; i++) {
			if (rand() % 2)
				cell[k][i].down = Open;
			else
				cell[k][i].down = Close;
			if (mn[k][i - 1] != mn[k][i] && mn[k][i] != mn[k][i + 1])
				cell[k][i].down = Close;
			else {
				if (mn[k][i] != mn[k][i + 1]) {
					j = i - 1;
					short flag = 1;
					while (mn[k][j] == mn[k][i]) {
						if (!cell[k][j].down) {
							flag = 0;
							break;
						}
						j--;
					}
					if (flag)
						cell[k][i].down = Close;
				}
			}
		}
		if (k<N - 1) {
			k++;
			for (i = 0; i<N; i++) {
				if (cell[k - 1][i].down) {
					mn[k][i] = 0;
					cell[k][i].up = Open;
				}
				else {
					mn[k][i] = mn[k - 1][i];
					cell[k][i].up = Close;
				}
				if (cell[k - 1][i].right)
					cell[k - 1][i + 1].left = Open;
				else
					cell[k - 1][i + 1].left = Close;
				cell[k][i].down = Close;
			}
			for (i = 0; i<N - 1; i++)
				cell[k][i].right = Close;
		}
		else {
			for (i = 0; i<N; i++) {
				cell[k][i].down = Open;
				if (mn[k][i] != mn[k][i + 1])
					cell[k][i].right = Close;
				cell[k][i].left = cell[k][i - 1].right;
			}
			cell[k][N - 1].right = Open;
			k++;
		}
	}
	cell[0][rand() % N].up = Close;
	cell[N - 1][rand() % N].down = Close;
}

void BrokenEller::faway() {
	short wave[N][N], i, j, start, fin, len;
	bool way[N][N];
	for (i = 0; i<N; i++)
		if (!cell[0][i].up) {
			wave[0][i] = N*N;
			start = i;
		}
		else
			wave[0][i] = N*N;
	for (i = 1; i<N; i++)
		for (j = 0; j<N; j++)
			wave[i][j] = N*N;
	for (i = 0; i<N; i++)
		if (!cell[N - 1][i].down) {
			fin = i;
			break;
		}
	setwave(wave, 0, start, 0, fin);
	printf("Shortest way = %d\n", wave[N - 1][fin]);
	for (i = 0; i<N; i++)
		for (j = 0; j<N; j++)
			way[i][j] = 0;
	way[N - 1][fin] = 1;
	i = N - 1;
	j = fin;
	len = wave[i][j];
	while (len) {
		if (wave[i - 1][j] == len - 1 && i>0 && !cell[i][j].up) {
			len--;
			i--;
			way[i][j] = 1;
		}
		if (wave[i][j - 1] == len - 1 && j>0 && !cell[i][j].left) {
			len--;
			j--;
			way[i][j] = 1;
		}
		if (wave[i][j + 1] == len - 1 && j<N - 1 && !cell[i][j].right) {
			len--;
			j++;
			way[i][j] = 1;
		}
		if (wave[i + 1][j] == len - 1 && i<N - 1 && !cell[i][j].down) {
			len--;
			i++;
			way[i][j] = 1;
		}
	}
	way[0][start] = 1;
}

void BrokenEller::setwave(short wave[][N], short i, short j, short value, short finish) {
	if (wave[i][j]>value && value<wave[N - 1][finish])
		wave[i][j] = value;
	else
		return;
	if (!cell[i][j].down)
		if (i<N - 1)
			setwave(wave, i + 1, j, value + 1, finish);
		else
			return;
	if (!cell[i][j].up && i>0)
		setwave(wave, i - 1, j, value + 1, finish);
	if (!cell[i][j].left && j>0)
		setwave(wave, i, j - 1, value + 1, finish);
	if (!cell[i][j].right && j<N - 1)
		setwave(wave, i, j + 1, value + 1, finish);
}