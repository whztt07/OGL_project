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
	BrokenEller lab;
	while (1) {
		system("CLS");
		puts("      MENU:      ");
		puts("  1 - MAKE LABIRINT  ");
		puts("  2 - FIND A WAY  ");
		puts("  0 - EXIT  ");
		c = _getch();
		switch (c) {
		case '1': lab.lab(); break;
		case '2': lab.faway(); break;
		case '0': return;
		default: puts("WRONG CASE");
		}
	}
}