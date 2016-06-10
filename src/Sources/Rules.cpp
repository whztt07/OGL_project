#include "graphics.h"
#include "Menu.h"
#include "Button.h"
extern int X_max;
extern int Y_max;

void Rules() {
	char name_menu[] = "Правила";
	char **null_body = NULL;
	Menu rule(name_menu, null_body, 0);
	rule.submenu_set_settings_and_draw_background_and_header();
	char str_buff[70];	// буффер для строчки
	int str_count = 0;	// количество строчек в файле
	FILE*  f;
	f = fopen("rules.txt", "r");
	if (f == 0) {
		char str_error[] = "Ошибка! Не удалось открыть файл с правилами";
		outtextxy((X_max - textwidth(str_error)) / 2, (Y_max - textheight(str_error)) / 2, str_error);
		delay(2000);
		return;
	}
	while (!feof(f)) {
		fgets(str_buff, 70, f);
		str_count++;
	}
	rewind(f);
	int height_rule = textheight("Правила");
	settextstyle(10, 0, 3);
	int interval = (Y_max - height_rule) / str_count - textheight("Правила") - 25;
	int temp_Y_text = height_rule + 50;
	setcolor(BLACK);
	for (int i = 0; i < str_count; i++) {
		fgets(str_buff, 70, f);
		outtextxy((X_max - textwidth(str_buff)) / 2, temp_Y_text, str_buff);
		temp_Y_text += interval + textheight(str_buff);
	}
	Button button;
	while (true) {
		button.set_flag_click(false);
		if (ismouseclick(WM_LBUTTONUP)) {
			clearmouseclick(WM_LBUTTONUP);
			button.set_flag_click(true);
		}
		if (button.draw_exit()) {
			break;
		}		
	}
}