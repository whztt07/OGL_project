#include "Button.h"
#include "graphics.h"
Button::Button() {
	// настраиваем кнопки по умолчанию
	Button::set_exit_style(COLOR(73, 72, 67), YELLOW, COLOR(180, 230, 0));
	Button::set_pause_style(COLOR(73, 72, 67), YELLOW, COLOR(180, 230, 0));
	Button::set_makemove_style(COLOR(73, 72, 67), YELLOW, COLOR(180, 230, 0));
	Button::set_exit_loc_size(0, 0, 40, 40);
	Button::set_pause_loc_size(43, 0, 40, 40);
	Button::set_makemove_loc_size(250, 540, 290, 60);
}

void Button::set_exit_style(int color_line, int color_norm_fill, int color_abnorm_fill) {
	Button::exit.style.color_line = color_line;
	Button::exit.style.color_norm_fill = color_norm_fill;
	Button::exit.style.color_abnorm_fill = color_abnorm_fill;
}
void Button::set_exit_loc_size(int left, int top, int sizeX, int sizeY) {
	Button::exit.loc_size.left = left;
	Button::exit.loc_size.top = top;
	Button::exit.loc_size.right = left + sizeX;
	Button::exit.loc_size.bottom = top + sizeY;
}
bool Button::draw_exit() {
	bool if_click = Button::set_fill_color_and_return_true_if_click(Button::exit);
	if (if_click)
		return true;
	setlinestyle(0, 0, 2);
	setcolor(Button::exit.style.color_line);
	rectangle(Button::exit.loc_size.left, Button::exit.loc_size.top, Button::exit.loc_size.right, Button::exit.loc_size.bottom);
	line(Button::exit.loc_size.left + 5, Button::exit.loc_size.top + 5, Button::exit.loc_size.right - 5, Button::exit.loc_size.bottom - 5);
	line(Button::exit.loc_size.right - 5, Button::exit.loc_size.top + 5, Button::exit.loc_size.left + 5, Button::exit.loc_size.bottom - 5);
	floodfill(Button::exit.loc_size.left + 2, Button::exit.loc_size.top + 2, Button::exit.style.color_line);
	return false;
}

void Button::set_pause_style(int color_line, int color_norm_fill, int color_abnorm_fill) {
	Button::pause.style.color_line = color_line;
	Button::pause.style.color_norm_fill = color_norm_fill;
	Button::pause.style.color_abnorm_fill = color_abnorm_fill;
}
void Button::set_pause_loc_size(int left, int top, int sizeX, int sizeY) {
	Button::pause.loc_size.left = left;
	Button::pause.loc_size.top = top;
	Button::pause.loc_size.right = left + sizeX;
	Button::pause.loc_size.bottom = top + sizeY;
}
bool Button::draw_pause() {
	bool if_click = Button::set_fill_color_and_return_true_if_click(Button::pause);
	if (if_click)
		return true;
	setlinestyle(0, 0, 2);
	setcolor(Button::pause.style.color_line);
	rectangle(Button::pause.loc_size.left, Button::pause.loc_size.top, Button::pause.loc_size.right, Button::pause.loc_size.bottom);
	line(Button::pause.loc_size.left + 15, Button::pause.loc_size.top + 7, Button::pause.loc_size.left + 15, Button::pause.loc_size.bottom - 7);
	line(Button::pause.loc_size.right - 15, Button::pause.loc_size.top + 7, Button::pause.loc_size.right - 15, Button::pause.loc_size.bottom - 7);
	floodfill(Button::pause.loc_size.left + 2, Button::pause.loc_size.top + 2, Button::pause.style.color_line);
	return false;
}

void Button::set_makemove_style(int color_line, int color_norm_fill, int color_abnorm_fill) {
	Button::makemove.style.color_line = color_line;
	Button::makemove.style.color_norm_fill = color_norm_fill;
	Button::makemove.style.color_abnorm_fill = color_abnorm_fill;
}
void Button::set_makemove_loc_size(int left, int top, int sizeX, int sizeY) {
	Button::makemove.loc_size.left = left;
	Button::makemove.loc_size.top = top;
	Button::makemove.loc_size.right = left + sizeX;
	Button::makemove.loc_size.bottom = top + sizeY;
}
bool Button::draw_makemove() {
	setcolor(Button::makemove.style.color_line);
	setlinestyle(0, 0, 1);
	bool if_click = Button::set_bk_color_and_return_true_if_click();
	if (if_click)
		return true;
	outtextxy((Button::makemove.loc_size.right - Button::makemove.loc_size.left - textwidth("Конец хода")) / 2 + Button::makemove.loc_size.left, Button::makemove.loc_size.top + 5, "Конец хода");
	return false;
}

void Button::set_flag_click(bool flag) {
	Button::flag_click = flag;
}

bool Button::set_fill_color_and_return_true_if_click(Button::type_button button) {
	int mx = mousex(), my = mousey();
	if (mx > button.loc_size.left && mx < button.loc_size.right &&
		my > button.loc_size.top && my < button.loc_size.bottom) {
		if (Button::flag_click)
			return true;
		setfillstyle(1, button.style.color_abnorm_fill);
	}
	else
		setfillstyle(1, button.style.color_norm_fill);
	return false;
}
bool Button::set_bk_color_and_return_true_if_click() {
	int mx = mousex(), my = mousey();
	if (mx > makemove.loc_size.left && mx < makemove.loc_size.right &&
		my > makemove.loc_size.top && my < makemove.loc_size.bottom) {
		if (Button::flag_click)
			return true;
		setbkcolor(makemove.style.color_abnorm_fill);
	}
	else
		setbkcolor(makemove.style.color_norm_fill);
	return false;
}

Button::~Button() {}
