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

void Savegame() {
	char header[] = "Сохранение";
	const int num_of_item_menu = 5;
	bool flag_is_click = false;
	int number_of_slot = -1;
	char* slot[num_of_item_menu];
	for (int i = 0; i < num_of_item_menu; i++) {
		slot[i] = new char[50];
		create_snapshot(i, slot[i]);
	}
	Menu Save_Menu(header, slot, 5);
	Button button;
	for (int i = 0; i < num_of_item_menu; i++)
		delete[] slot[i];
	Save_Menu.submenu_set_settings_and_draw_background_and_header();
	while (true) {
		button.set_flag_click(false);
		flag_is_click = false;
		if (ismouseclick(WM_LBUTTONUP)) {
			clearmouseclick(WM_LBUTTONUP);
			button.set_flag_click(true);
			flag_is_click = true;
		}
		if (button.draw_exit()) {
			return;
		}
		number_of_slot = Save_Menu.draw_body(mousex(), mousey(), flag_is_click);
		switch (number_of_slot)
		{
		case 0:	Savedata(0, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music); return;
		case 1: Savedata(1, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music); return;
		case 2: Savedata(2, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music); return;
		case 3:	Savedata(3, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music); return;
		case 4: Savedata(4, nim.game_mode, nim.difficulty, bun.form, nim.array_bunches, nim.who_do_move, str_music); return;
		default: break;
		}
	}
}
