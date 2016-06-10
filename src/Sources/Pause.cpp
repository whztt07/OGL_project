#include "Menu.h"
#include <string.h>
#include "secondary_functions.h"
#include "graphics.h"
#include "Bunches.h"
#include "Save.h"
#include "func_for_menu.h"

extern Bunches bun;
extern Logic_Nim nim;
extern PF array_func[];
extern char str_music[50];

bool Pause() {
	char it_is_pause[] = "Пауза";
	const int num_of_item_menu = 5;
	bool flag_is_click = false;
	int number_of_func = -1;
	char* arr_str_menu[num_of_item_menu];
	for (int i = 0; i < num_of_item_menu; i++) {
		arr_str_menu[i] = new char[17];
	}
	strcpy(arr_str_menu[0], "Продолжить");
	strcpy(arr_str_menu[1], "Сохранить игру");
	strcpy(arr_str_menu[2], "Настройки");
	strcpy(arr_str_menu[3], "Правила игры");
	strcpy(arr_str_menu[4], "Выход в меню");
	Menu Pause_Menu(it_is_pause, arr_str_menu, 5);
	for (int i = 0; i < num_of_item_menu; i++)
		delete[] arr_str_menu[i];

	Pause_Menu.menu_set_settings_and_draw_background_and_header();
	while (true) {
		flag_is_click = false;
		if (ismouseclick(WM_LBUTTONUP)) {
			clearmouseclick(WM_LBUTTONUP);
			flag_is_click = true;
		}
		number_of_func = Pause_Menu.draw_body(mousex(), mousey(), flag_is_click);
		switch (number_of_func)	{
		case 0:	return false; /*Продолжить игру*/
		case 1: getstate__func__putstate(array_func[4]); break; /*Сохранить игру*/
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
		case 4:	return true; /*Выход в главное меню*/
		default: break;
		}
	}
}