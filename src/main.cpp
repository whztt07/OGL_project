#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "broken_eller.h"

void menu(void);

int main(int argc, char** argv) {
	menu();
}

void menu() {
	char c;
	BrokenEller Maze;
	while (1) {
		system("CLS");
		puts("      MENU:      ");
		puts("  1 - MAKE LABIRINT  ");
		puts("  2 - FIND A WAY  ");
		puts("  ESC - EXIT  ");
		c = _getch();
		switch (c) {
			case '1': Maze.MakeLabirint(); break;
			case '2': Maze.FindTheWay(); break;
			case 27 : return;
			default: puts("WRONG CASE");
		}
	}
}