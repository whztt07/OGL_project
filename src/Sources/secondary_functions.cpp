#include "string.h"
#include "graphics.h"
#include "Logic_Nim.h"
#include "secondary_functions.h"
#include "Bunches.h"
#include "Menu.h"
extern int X_max;
extern int Y_max;

bool choose_mode(Logic_Nim &nim) {
	char name_header[] = "Выберите режим игры";
	char *body[2];
	for (int i = 0; i < 2; i++)
		body[i] = new char[25];
	strcpy(body[0], "Игрок VS Компьютер");
	strcpy(body[1], "Игрок VS Игрок");
	Menu diffic(name_header, body, 2);
	for (int i = 0; i < 2; i++)
		delete[] body[i];
	diffic.submenu_set_settings_and_draw_background_and_header();
	diffic.set_starting_settings_for_output_of_body(0, 200, X_max, Y_max - 200);
	bool flag_click = false;
	int number_menu = -1;
	while (true) {
		flag_click = false;
		if (ismouseclick(WM_LBUTTONUP)) {
			clearmouseclick(WM_LBUTTONUP);
			flag_click = true;
		}
		number_menu = diffic.draw_body(mousex(), mousey(), flag_click);
		switch (number_menu) {
		case 0: nim.game_mode = Logic_Nim::bot_vs_player; nim.who_do_move = Logic_Nim::player; return true;
		case 1: nim.game_mode = Logic_Nim::player1_vs_player2; nim.who_do_move = Logic_Nim::player1; return false;
		default: break;
		}
	}
	return false;
}

void choose_difficulty(Logic_Nim &nim){
	char name_header[] = "Выберите сложность игры";
	char *body[2];
	for (int i = 0; i < 2; i++)
		body[i] = new char[10];
	strcpy(body[0], "Мастер");
	strcpy(body[1], "Новичок");
	Menu diffic(name_header, body, 2);
	for (int i = 0; i < 2; i++)
		delete[] body[i];
	diffic.submenu_set_settings_and_draw_background_and_header();
	diffic.set_starting_settings_for_output_of_body(0, 200, X_max, Y_max - 200);
	bool flag_click = false;
	int number_menu = -1;
	while (true) {
		flag_click = false;
		if (ismouseclick(WM_LBUTTONUP)) {
			clearmouseclick(WM_LBUTTONUP);
			flag_click = true;
		}
		number_menu = diffic.draw_body(mousex(), mousey(), flag_click);
		switch (number_menu) {
		case 0: nim.difficulty = Logic_Nim::hard; return;
		case 1: nim.difficulty = Logic_Nim::easy; return;
		default: break;
		}
	}
}



void draw_who_do_move(Logic_Nim &nim, Bunches &bun, int color_text) {
	settextstyle(10, 0, 4);
	setcolor(BLUE);
	setbkcolor(bun.color_background);
	setfillstyle(1, bun.color_background);
	char str[] = "Ходит компьютер"; // выбранно именно это значение, как максимально длинная строчка
	bar((X_max - textwidth(str)) / 2 - 1, 0, (X_max - textwidth(str)) / 2 + textwidth(str) + 1, textheight(str));
	switch (nim.who_do_move)
	{
	case Logic_Nim::bot:
		outtextxy((X_max - textwidth("Ходит компьютер")) / 2, 0, "Ходит компьютер");
		break;
	case Logic_Nim::player:
		outtextxy((X_max - textwidth("Ваш ход")) / 2, 0, "Ваш ход");
		break;
	case Logic_Nim::player1:
		outtextxy((X_max - textwidth("Игрок _ 1")) / 2, 0, "Игрок _ 1");
		break;
	case Logic_Nim::player2:
		outtextxy((X_max - textwidth("Игрок _ 2")) / 2, 0, "Игрок _ 2");
		break;
	default:
		break;
	}
}

void draw_end(Logic_Nim &nim, Bunches &bun) {
	struct textsettingstype t;
	gettextsettings(&t);
	settextstyle(10, 0, 5);
	setbkcolor(bun.color_background);
	switch (nim.who_do_move)
	{
	case Logic_Nim::bot: 
		outtextxy((X_max - textwidth("Вы проиграли")) / 2, 200, "Вы проиграли");
		break;
	case Logic_Nim::player:
		outtextxy((X_max - textwidth("Вы выиграли")) / 2, 200, "Вы выиграли");
		break;
	case Logic_Nim::player1:
		outtextxy((X_max - textwidth("Выиграл Игрок_1")) / 2, 200, "Выиграл Игрок_1");
		break;
	case Logic_Nim::player2:
		outtextxy((X_max - textwidth("Выиграл Игрок_2")) / 2, 200, "Выиграл Игрок_2");
		break;
	default:
		break;
	}
	settextstyle(t.font, t.direction, t.charsize);
}


void getstate__func__putstate(void (*function)()) {
	void* screen = malloc(imagesize(0, 0, X_max, Y_max));
	getimage(0, 0, X_max, Y_max, screen);
	textsettingstype textsettings;
	gettextsettings(&textsettings);
	linesettingstype lineinfo;
	getlinesettings(&lineinfo);
	int color_bk = getbkcolor();
	int color = getcolor();
	fillsettingstype fillsettings;
	getfillsettings(&fillsettings);

	(*function)();
	
	settextstyle(textsettings.font, textsettings.direction, textsettings.charsize);
	setlinestyle(lineinfo.linestyle, lineinfo.upattern, lineinfo.thickness);
	setbkcolor(color_bk);
	setcolor(color);
	setfillstyle(fillsettings.pattern, fillsettings.color);
	putimage(0, 0, screen, 0);
	free(screen);
}
bool getstate__func__putstate(bool(*function)()) {
	void* screen = malloc(imagesize(0, 0, X_max, Y_max));
	getimage(0, 0, X_max, Y_max, screen);
	textsettingstype textsettings;
	gettextsettings(&textsettings);
	linesettingstype lineinfo;
	getlinesettings(&lineinfo);
	int color_bk = getbkcolor();
	int color = getcolor();
	fillsettingstype fillsettings;
	getfillsettings(&fillsettings);

	bool value = (*function)();

	settextstyle(textsettings.font, textsettings.direction, textsettings.charsize);
	setlinestyle(lineinfo.linestyle, lineinfo.upattern, lineinfo.thickness);
	setbkcolor(color_bk);
	setcolor(color);
	setfillstyle(fillsettings.pattern, fillsettings.color);
	putimage(0, 0, screen, 0);
	free(screen);
	return value;
}

