#include <fstream>
#include <iostream>
#include "graphics.h"
#include "Menu.h"
#include "Logic_Nim.h"
#include "Bunches.h"
#include "Button.h"
#include "secondary_functions.h"
#include "Save.h"

using namespace std;
extern int X_max;
extern int Y_max;
extern Bunches bun;
extern Logic_Nim nim;
extern char str_music[];


bool Loadgame(Logic_Nim &nim, Bunches &bun) {
	char header[] = "Загрузка";
	const int num_of_item_menu = 5;
	bool flag_is_click = false;
	int number_of_slot = -1;
	char* slot[num_of_item_menu];
	for (int i = 0; i < num_of_item_menu; i++) {
		slot[i] = new char[50];
		create_snapshot(i, slot[i]);
	}
	Menu Load_Menu(header, slot, 5);
	Button button;
	for (int i = 0; i < num_of_item_menu; i++)
		delete[] slot[i];
	Load_Menu.submenu_set_settings_and_draw_background_and_header();
	while (true) {
		button.set_flag_click(false);
		flag_is_click = false;
		if (ismouseclick(WM_LBUTTONUP)) {
			clearmouseclick(WM_LBUTTONUP);
			button.set_flag_click(true);
			flag_is_click = true;
		}
		if (button.draw_exit())
			return false;
	
		number_of_slot = Load_Menu.draw_body(mousex(), mousey(), flag_is_click);
		switch (number_of_slot) {
		case 0:	if (Loaddata(0, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music))
					return true;
			break;
		case 1:	if (Loaddata(1, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music))
					return true;
			break;
		case 2:	if (Loaddata(2, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music))
					return true;
			break;
		case 3:	if (Loaddata(3, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music))
					return true;
			break;
		case 4:	if (Loaddata(4, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music))
					return true;
			break;
		default: break;
		}
	}
	return false;
}
