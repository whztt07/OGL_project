#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "growing_tree.h"

void menu(void);

int main(int argc, char** argv) {
	menu();
}

void menu() {
	char c;
	GrowingTree Maze(COMMON_CELL_COUNT, COMMON_CELL_COUNT);
	while (1) {
		system("CLS");
		puts("      MENU:      ");
		puts("  1 - MAKE LABIRINT  ");
		puts("  2 - FIND A WAY (NOT WORKING) ");
		puts("  ESC - EXIT  ");
		c = _getch();
		switch (c) {
			case '1': Maze.MakeLabyrinth(); break;
			case '2': break;
			case 27 : return;
			default: puts("WRONG CASE");
		}
	}
}