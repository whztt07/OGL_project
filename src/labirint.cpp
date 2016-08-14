#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>

const short N = 20;
const short CSize = 25;
const short Cntr = 5;
const short X_max = N*CSize + 2 * Cntr;
const short Y_max = N*CSize + 2 * Cntr;

bool right[N][N];
bool left[N][N];
bool up[N][N];
bool down[N][N];

void menu(void);
void lab(void);
void faway(void);
void setwave(short[N][N], short, short, short, short);

int main(int argc, char** argv) {
	menu();
}

void menu() {
	char c;
	while (1) {
		system("CLS");
		puts("      MENU:      ");
		puts("  1 - MAKE LABIRINT  ");
		puts("  2 - FIND A WAY  ");
		puts("  0 - EXIT  ");
		c = _getch();
		switch (c) {
		case '1': lab(); break;
		case '2': faway(); break;
		case '0': closegraph(); return;
		default: puts("WRONG CASE");
		}
	}
}

void lab() {
	short i, j, mn[N][N], uniq = 1, k = 0;
	srand(time(0));
	for (i = 0; i<N; i++) {
		right[i][N - 1] = 1;
		left[i][0] = 1;
	}
	for (i = 0; i<N; i++) {
		mn[0][i] = 0;
		up[0][i] = 1;
	}
	while (k<N) {
		for (i = 0; i<N; i++)
			if (!mn[k][i])
				mn[k][i] = uniq++;
		for (i = 0; i<N - 1; i++) {
			if (mn[k][i] == mn[k][i + 1])
				right[k][i] = 1;
			else {
				if (rand() % 2)
					right[k][i] = 1;
				else {
					right[k][i] = 0;
					mn[k][i + 1] = mn[k][i];
				}
			}
		}
		for (i = 0; i<N; i++) {
			if (rand() % 2)
				down[k][i] = 1;
			else
				down[k][i] = 0;
			if (mn[k][i - 1] != mn[k][i] && mn[k][i] != mn[k][i + 1])
				down[k][i] = 0;
			else {
				if (mn[k][i] != mn[k][i + 1]) {
					j = i - 1;
					short flag = 1;
					while (mn[k][j] == mn[k][i]) {
						if (!down[k][j]) {
							flag = 0;
							break;
						}
						j--;
					}
					if (flag)
						down[k][i] = 0;
				}
			}
		}
		if (k<N - 1) {
			k++;
			for (i = 0; i<N; i++) {
				if (down[k - 1][i]) {
					mn[k][i] = 0;
					up[k][i] = 1;
				}
				else {
					mn[k][i] = mn[k - 1][i];
					up[k][i] = 0;
				}
				if (right[k - 1][i])
					left[k - 1][i + 1] = 1;
				else
					left[k - 1][i + 1] = 0;
				down[k][i] = 0;
			}
			for (i = 0; i<N - 1; i++)
				right[k][i] = 0;
		}
		else {
			for (i = 0; i<N; i++) {
				down[k][i] = 1;
				if (mn[k][i] != mn[k][i + 1])
					right[k][i] = 0;
				left[k][i] = right[k][i - 1];
			}
			right[k][N - 1] = 1;
			k++;
		}
	}
	up[0][rand() % N] = 0;
	down[N - 1][rand() % N] = 0;
	closegraph();
	initwindow(X_max, Y_max);
	for (i = 0; i<N; i++)
		for (j = 0; j<N; j++) {
			if (up[i][j]) line(j*CSize + Cntr, i*CSize + Cntr, (j + 1)*CSize + Cntr, i*CSize + Cntr);
			if (down[i][j]) line(j*CSize + Cntr, (i + 1)*CSize + Cntr, (j + 1)*CSize + Cntr, (i + 1)*CSize + Cntr);
			if (left[i][j]) line(j*CSize + Cntr, i*CSize + Cntr, j*CSize + Cntr, (i + 1)*CSize + Cntr);
			if (right[i][j]) line((j + 1)*CSize + Cntr, i*CSize + Cntr, (j + 1)*CSize + Cntr, (i + 1)*CSize + Cntr);
		}
}

void faway() {
	short wave[N][N], i, j, start, fin, len;
	bool way[N][N];
	for (i = 0; i<N; i++)
		if (!up[0][i]) {
			wave[0][i] = N*N;
			start = i;
		}
		else
			wave[0][i] = N*N;
	for (i = 1; i<N; i++)
		for (j = 0; j<N; j++)
			wave[i][j] = N*N;
	for (i = 0; i<N; i++)
		if (!down[N - 1][i]) {
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
		if (wave[i - 1][j] == len - 1 && i>0 && !up[i][j]) {
			len--;
			i--;
			way[i][j] = 1;
		}
		if (wave[i][j - 1] == len - 1 && j>0 && !left[i][j]) {
			len--;
			j--;
			way[i][j] = 1;
		}
		if (wave[i][j + 1] == len - 1 && j<N - 1 && !right[i][j]) {
			len--;
			j++;
			way[i][j] = 1;
		}
		if (wave[i + 1][j] == len - 1 && i<N - 1 && !down[i][j]) {
			len--;
			i++;
			way[i][j] = 1;
		}
	}
	way[0][start] = 1;
	setfillstyle(1, 14);
	for (i = 0; i<N; i++)
		for (j = 0; j<N; j++) {
			if (way[i][j])
				bar(j*CSize + Cntr + CSize / 4, i*CSize + Cntr + CSize / 4, (j + 1)*CSize + Cntr - CSize / 4, (i + 1)*CSize + Cntr - CSize / 4);
		}
}

void setwave(short wave[][N], short i, short j, short value, short finish) {
	if (wave[i][j]>value && value<wave[N - 1][finish])
		wave[i][j] = value;
	else
		return;
	if (!down[i][j])
		if (i<N - 1)
			setwave(wave, i + 1, j, value + 1, finish);
		else
			return;
	if (!up[i][j] && i>0)
		setwave(wave, i - 1, j, value + 1, finish);
	if (!left[i][j] && j>0)
		setwave(wave, i, j - 1, value + 1, finish);
	if (!right[i][j] && j<N - 1)
		setwave(wave, i, j + 1, value + 1, finish);
}