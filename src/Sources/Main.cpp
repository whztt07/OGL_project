#pragma comment(lib,"Winmm.lib")

#include <string.h>
#include "graphics.h"
#include "Menu.h"
#include "Button.h"
#include "func_for_menu.h"
#include "secondary_functions.h"
#include "Logic_Nim.h"
#include "Bunches.h"
#include "settings_config.h"
int X_max = 800;
int Y_max = 600;
PF array_func[] = { &New_game, &Continue_game, &Settings, &Rules, &Savegame };
Bunches bun;
Logic_Nim nim;

char str_music[50];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {	
	get_settings(bun, str_music);
	PlaySound(str_music, NULL, SND_ASYNC | SND_FILENAME);
	char game_name[] = "Ним";
	const int num_of_item_menu = 5;
	bool flag_exit = false;
	bool flag_is_click = false;
	int number_of_func = -1;
	char* arr_str_menu[num_of_item_menu];
	for (int i = 0; i < num_of_item_menu; i++) {
		arr_str_menu[i] = new char[17];
	}
	strcpy(arr_str_menu[0], "Новая игра");
	strcpy(arr_str_menu[1], "Продолжить игру");
	strcpy(arr_str_menu[2], "Настройки");
	strcpy(arr_str_menu[3], "Правила игры");
	strcpy(arr_str_menu[4], "Выход");
	
	initwindow(X_max, Y_max, "Игра Ним", 200, 0);
	Menu Main_Menu(game_name, arr_str_menu, num_of_item_menu);
	for (int i = 0; i < num_of_item_menu; i++)
		delete[] arr_str_menu[i];
	Main_Menu.set_color_background(GREEN);
	Main_Menu.draw_background();
	Main_Menu.set_style_headline(YELLOW, 8, 10);
	Main_Menu.draw_headline();
	Main_Menu.set_style_body(COLOR(50, 205, 50), COLOR(174, 235, 174), 5, 10);
	while (!flag_exit) {
		flag_is_click = false;
		if (ismouseclick(WM_LBUTTONUP)) {
			clearmouseclick(WM_LBUTTONUP);
			flag_is_click = true;
		}
		number_of_func = Main_Menu.draw_body(mousex(), mousey(), flag_is_click);
		switch (number_of_func)	{
		case 0:	getstate__func__putstate(array_func[0]); break; /*Новая игра*/
		case 1:	getstate__func__putstate(array_func[1]); break; /*Продолжить игру*/
		case 2:	
			char str_past[50];
			strcpy(str_past, str_music);
			getstate__func__putstate(array_func[2]);  
			if (strcmp(str_music, str_past)) {
				PlaySound(NULL, NULL, SND_ASYNC | SND_FILENAME);
				PlaySound(str_music, NULL, SND_ASYNC | SND_FILENAME);
			}
			break;
		case 3:	getstate__func__putstate(array_func[3]); break; /*Правила игры*/
		case 4:	flag_exit = true; break; /*Выход*/
		default: break;
		}
	}	
	closegraph();
	PlaySound(NULL, NULL, SND_ASYNC | SND_FILENAME);
	return 0;
}