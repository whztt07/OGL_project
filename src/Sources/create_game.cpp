#include "Button.h"
#include "Logic_Nim.h"
#include "graphics.h"
#include "Bunches.h"
#include "secondary_functions.h"

extern int X_max;
extern int Y_max;
extern Bunches bun;
extern Logic_Nim nim;

void create_game() {
	Button butts;
	setfillstyle(1, COLOR(167, 200, 0));
	bar(0, 0, X_max, Y_max);
	bun.draw_razm();
	bun.set_mouse_click(false);
	bun.draw_all(nim);
	draw_who_do_move(nim, bun, RED);
	while (true) {
		bun.set_mouse_click(false);
		butts.set_flag_click(false);
		if (ismouseclick(WM_LBUTTONUP)) {
			clearmouseclick(WM_LBUTTONUP);
			bun.set_mouse_click(true);
			butts.set_flag_click(true);
		}
		if (butts.draw_exit())
			break;
		if (butts.draw_pause()) {
			bool(*pointer_func)() = &Pause;
			Bunches::figure past_figure = bun.form;
			if (getstate__func__putstate(pointer_func))
				return;
			if (bun.form != past_figure)
				bun.clear_fild(nim);
			bun.set_mouse_click(false);
		}

		bun.draw_all(nim);
		if (butts.draw_makemove() && nim.chosen_bunch >= 0 && nim.chosen_bunch <= nim.num_of_bunches) {
			bun.delete_chosing_stones(nim);
			nim.do_turn();
			if (nim.end_of_game()) {
				draw_end(nim, bun);
				delay(1000);
				break;
			}
			nim.swap_priority();
			nim.set_num_chosen_bunch_stone(-1, -1);
			draw_who_do_move(nim, bun, RED);
			if (nim.who_do_move == Logic_Nim::bot) {
				nim.bot_choosing();
				bun.draw_all(nim);
				delay(1500);
				bun.delete_chosing_stones(nim);
				nim.do_turn();
				if (nim.end_of_game()) {
					draw_end(nim, bun);
					delay(1000);
					break;
				}
				nim.swap_priority();
				nim.set_num_chosen_bunch_stone(-1, -1);
				draw_who_do_move(nim, bun, RED);
	}}}
}