#include "Bunches.h"
#include "Button.h"
#include "graphics.h"
#include "menu.h"
#include "settings_config.h"

using namespace std;
 Button butts;
extern Bunches bun;
extern Logic_Nim nim;
extern int X_max;
extern int Y_max;
extern char str_music[50];
extern char str_music1[];
extern char str_music2[];
extern char str_music3[];
extern char str_music4[];
extern char str_music5[];
extern char str_music_file1[];
extern char str_music_file2[];
extern char str_music_file3[];
extern char str_music_file4[];
extern char str_music_file5[];
const int cv_text_abnormal = LIGHTCYAN; // цвет текста выбранного элемента меню
const int cv_text_normal = COLOR(77, 77, 77); // цвет текста не выбранного элемента меню
const int color_background = COLOR(52, 201, 36);
const int color_header = COLOR(255, 255, 51);

// рисует фигуры (0 - triangle, 1 - circle, 2 - square)
void figure_draw(int x, int y, int size, int form) {
	switch (form) {
		case 0: {
			line(x, y - 1, x + size / 2, y - size);
			line(x + size / 2, y - size, x + size, y - 1);
			line(x, y, x + size, y);
			break;
		}
		case 1: {
			circle(x + size / 2, y - size / 2, size / 2);
			break;
		}
		case 2: {
			line(x, y, x, y - size);
			line(x, y - size, x + size, y - size);
			line(x + size, y - size, x + size, y);
			line(x + size, y, x, y);
			break;
		}
	}
}

// опреледяет, находится ли курсор в фигуре (0 - triangle, 1 - circle, 2 - square)
bool figure_is_in_area(int x, int y, int size, int form, int mx, int my) {
	switch (form) {
		case 0: {
			int x2 = x + size, y2 = y, x3 = x + size / 2, y3 = y - size;
			if ((((mx - x) * (y2 - y) - (my - y) * (x2 - x)) * ((x3 - x) * (y2 - y) - (y3 - y) * (x2 - x)) >= 0))
				if ((((mx - x2) * (y3 - y2) - (my - y2) * (x3 - x2)) * ((x - x2) * (y3 - y2) - (y - y2) * (x3 - x2)) >= 0))
					if ((((mx - x3) * (y - y3) - (my - y3) * (x - x3)) * ((x2 - x3) * (y - y3) - (y2 - y3) * (x - x3)) >= 0))
						return true;
			return false;
		}
		case 1: {
			x += size / 2;
			y -= size / 2;
			if (((mx - x) * (mx - x) + (my - y) * (my - y)) <= (size / 2) * (size / 2))
				return true;
			return false;
		}
		case 2: {
			if ((mx > x) && (mx < x + size) && (my < y) && (my > y - size))
				return true;
			return false;
		}
	}
	return false;
}

// рисует стрелку (0 - вверх, 1 - вниз)
void arrow_draw(int x, int y,int width, int height, bool position, int color) {
	setcolor(color);
	switch (position) {
		case true: {
			line(x + width / 2, y, x, y - height / 2);
			line(x + width / 2, y, x + width, y - height / 2);
			line(x, y - height / 2, x + width / 4, y - height / 2);
			line(x + width, y - height / 2, x + width - width / 4, y - height / 2);
			line(x + width / 4, y - height / 2, x + width / 4, y - height);
			line(x + width - width / 4, y - height / 2, x + width - width / 4, y - height);
			line(x + width / 4, y - height, x + width - width / 4, y - height);
			break;
		}
		case false: {
			line(x + width / 4, y, x + width - width / 4, y);
			line(x + width / 4, y, x + width / 4, y - height / 2);
			line(x + width - width / 4, y, x + width - width / 4, y - height / 2);
			line(x, y - height / 2, x + width / 4, y - height / 2);
			line(x + width - width / 4, y - height / 2, x + width, y - height / 2);
			line(x, y - height / 2, x + width / 2, y - height);
			line(x + width, y - height / 2, x + width / 2, y - height);
			break;
		}
	}
}

// определяет, находится ли курсор в тексте
bool Is_in_area(int mx, int my, char* str, int x, int y) {
	if (mx >= x && mx <= (x + textwidth(str)) && my >= y && my <= (y + textheight(str)))
		return true;
	return false;
}

// выделяет строку с музыкой, если на неё наведен курсор
void music_is_in_area(int x, int y, int mx, int my, char* str_mus, char* str_music_file, bool &flag_music_draw, bool &flag_music_click, bool flag_click) {
	if (Is_in_area(mx, my, str_mus, x, y)) {
		if (flag_click) {
			put_settings(bun.form, str_music_file);
			get_settings(bun, str_music);
			flag_music_draw = false;
			setcolor(color_background);
			arrow_draw(629, 279, 26, 26, false, color_background);
			flag_music_click = true;
		}
		setcolor(cv_text_abnormal);
	}
	else
		setcolor(cv_text_normal);
	outtextxy(x, y, str_mus);
}

// рисует список музыки
void music_list_draw(int x1, int y1, int x2, int y2, int mx, int my, bool &flag_music_draw, bool &flag_music_click, bool flag_click) {
	int interval = (y2 - y1) / 4;
	line(x1, y1 + interval, x1, y1 + 2);
	line(x2, y1 + interval, x2, y1 + 2);
	rectangle(x1, y1 + interval, x2, y2 + interval);
	for (int i = 1; i < 5; i++)
		line(x1, y1 + interval * i, x2, y1 + interval * i);
	if (flag_music_draw) {
		music_is_in_area(x1 + (x2 - x1 - textwidth(str_music1)) / 2, y1 + (interval - textheight(str_music1)) / 2, mx, my, str_music1, str_music_file1, flag_music_draw, flag_music_click, flag_click);
		music_is_in_area(x1 + (x2 - x1 - textwidth(str_music2)) / 2, y1 + ((interval - textheight(str_music2)) / 2) + interval, mx, my, str_music2, str_music_file2, flag_music_draw, flag_music_click, flag_click);
		music_is_in_area(x1 + (x2 - x1 - textwidth(str_music3)) / 2, y1 + ((interval - textheight(str_music3)) / 2) + interval * 2,  mx, my, str_music3, str_music_file3, flag_music_draw, flag_music_click, flag_click);
		music_is_in_area(x1 + (x2 - x1 - textwidth(str_music4)) / 2, y1 + ((interval - textheight(str_music4)) / 2) + interval * 3,  mx, my, str_music4, str_music_file4, flag_music_draw, flag_music_click, flag_click);
		music_is_in_area(x1 + (x2 - x1 - textwidth(str_music5)) / 2, y1 + ((interval - textheight(str_music5)) / 2) + interval * 4, mx, my, str_music5, str_music_file5, flag_music_draw, flag_music_click, flag_click);
	}
	else {
		outtextxy(x1 + (x2 - x1 - textwidth(str_music1)) / 2, y1 + ((interval - textheight(str_music1)) / 2), str_music1);
		outtextxy(x1 + (x2 - x1 - textwidth(str_music2)) / 2, y1 + ((interval - textheight(str_music2)) / 2) + interval, str_music2);
		outtextxy(x1 + (x2 - x1 - textwidth(str_music3)) / 2, y1 + ((interval - textheight(str_music3)) / 2) + interval * 2, str_music3);
		outtextxy(x1 + (x2 - x1 - textwidth(str_music4)) / 2, y1 + ((interval - textheight(str_music4)) / 2) + interval * 3, str_music4);
		outtextxy(x1 + (x2 - x1 - textwidth(str_music5)) / 2, y1 + ((interval - textheight(str_music5)) / 2) + interval * 4, str_music5);
	}
}

void figure_list_draw(int x_figure, int y_figure, int figure_size, int form, bool flag_click, int mx, int my) {
	if (figure_is_in_area(x_figure, y_figure, figure_size, form, mx, my)) {
		if (flag_click) {
			put_settings(form, str_music);
			get_settings(bun, str_music);
		}
		setcolor(cv_text_abnormal);
	}
	else {
		if (bun.form == form)
			setcolor(YELLOW);
		else
			setcolor(cv_text_normal);
	}
	figure_draw(x_figure, y_figure, figure_size, form);
}

void Settings() {
	setfillstyle(1, color_background);
	setbkcolor(color_background);
	settextstyle(10, 0, 4);
	bar(0, 0, X_max, Y_max);  // заливка фона
	int mx = 0, my = 0;	// координаты указателя мыши
	int text_x = (X_max - textwidth("Форма камней")) / 2 - 190;  // координата X для пункта меню
	int text_y = 150;  // координата Y для пункта меню
	int interval = 100; // расстояние между строками меню
	int interval_figure = 30; // расстояние между фигурами
	int figure_size = 70; // размер фигуры
	int x_figure = text_x + textwidth("Форма камней:") + interval_figure;  // координата X для первой фигуры 
	int y_figure = text_y + textheight("Форма камней:");  // координата Y для первой фигуры
	int music_bar_x1 = text_x + textwidth("Музыка:") + 10; // координа X1 для отображения первой мелодии
	int music_bar_x2 = text_x + textwidth("Форма камней:") + figure_size * 3 + interval_figure * 3; // координа Y1 для отображения первой мелодии
	int music_bar_y1 = text_y + interval; // координа X2 для отображения первой мелодии
	int music_bar_y2 = text_y + interval + textheight("Музыка:"); // координа Y2 для отображения первой мелодии
	int music_list_y1 = music_bar_y1 + (music_bar_y2 - music_bar_y1); // координата Y1 для списка с музыкой
	int music_list_y2 = music_bar_y1 + (music_bar_y2 - music_bar_y1) * 5; // координата Y2 для списка с музыкой
	int arrow_size = textheight("Музыка:") - 6; // размер стрелки
	int arrow_x = music_bar_x2 - arrow_size - 3; // координата X для стрелки
	int arrow_y = music_bar_y2 - 3; // координата Y для стрелки
	int text_current = COLOR(166, 83, 23);
	bool flag_arrow = false;
	bool flag_click = false;
	bool flag_arrow_up = false;
	bool flag_music_draw = false;
	bool flag_music_click = false;
	setlinestyle(0, 0, 1);
	settextstyle(10, 0, 6);
	setcolor(color_header);
	outtextxy((X_max - textwidth("Настройки игры")) / 2, 10, "Настройки игры");
	setlinestyle(0, 0, 2);
	settextstyle(10, 0, 4);
	setcolor(cv_text_normal);
	outtextxy(text_x, text_y, "Форма камней:");
	outtextxy(text_x, text_y + interval, "Музыка:");
	rectangle(music_bar_x1, music_bar_y1, music_bar_x2, music_bar_y2);
	line(music_bar_x2 - textheight("Музыка:"), music_bar_y1, music_bar_x2 - textheight("Музыка:"), music_bar_y2);
	settextstyle(10, 0, 3);
	get_settings(bun, str_music);
	while (true) {
		mx = mousex();
		my = mousey();
		flag_click = false;  // флаг, отвечающий за нажатие кнопки мыши
		butts.set_flag_click(false);
		if (ismouseclick(WM_LBUTTONUP)) {
			clearmouseclick(WM_LBUTTONUP);
			flag_click = true;
			butts.set_flag_click(true);
		}
		if (kbhit())
			if (getch() == 27)
				break;
		if (butts.draw_exit())
			break;
		figure_list_draw(x_figure, y_figure, figure_size, 0, flag_click, mx, my);
		figure_list_draw(x_figure + figure_size + interval_figure, y_figure, figure_size, 2, flag_click, mx, my);
		figure_list_draw(x_figure + figure_size * 2 + interval_figure * 2, y_figure, figure_size, 1, flag_click, mx, my);
		setcolor(cv_text_normal);
		if (figure_is_in_area(arrow_x, arrow_y, arrow_size, 2, mx, my)) {
			if (!flag_arrow) {
				if (flag_arrow_up) {
					setcolor(color_background);
					arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, false, color_background);
				}
				setcolor(cv_text_abnormal);
				arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, true, cv_text_abnormal);
				flag_arrow_up = false;
			}
			else {
				if (!flag_arrow_up) {
					setcolor(color_background);
					arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, true, color_background);
				}
				setcolor(cv_text_abnormal);
				arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, false, cv_text_abnormal);
				flag_arrow_up = true;
			}
			if (flag_click) {
				if (!flag_arrow) {
						setcolor(color_background);
						arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, false, color_background);
						setcolor(cv_text_normal);
						arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, true, cv_text_normal);
						flag_arrow = true;
					}
				else {
					setcolor(color_background);
					arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, true, color_background);
					setcolor(cv_text_normal);
					arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, false, cv_text_normal);
					flag_arrow = false;
				}
			}
		}
		else {
			if (!flag_arrow) {
				setcolor(cv_text_normal);
				arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, true, cv_text_normal);
			}
			else {
				setcolor(cv_text_normal);
				arrow_draw(arrow_x, arrow_y, arrow_size, arrow_size, false, cv_text_normal);
			}
		}
		if (flag_arrow) {
			setcolor(cv_text_normal);
			music_list_draw(music_bar_x1, music_list_y1, music_bar_x2, music_list_y2, mx, my, flag_arrow, flag_music_click, flag_click);
			flag_music_draw = true;
		}
		else {
			setcolor(color_background);
			music_list_draw(music_bar_x1, music_list_y1, music_bar_x2, music_list_y2, mx, my, flag_arrow, flag_music_click, flag_click);
			flag_music_draw = false;
		}
		if (!strcmp(str_music, str_music_file1)) {
			setcolor(text_current);
			outtextxy(music_bar_x1 + (music_bar_x2 - music_bar_x1 - textwidth(str_music1)) / 2, music_bar_y1 + 2, str_music1);
		}
		if (!strcmp(str_music, str_music_file2)) {
			setcolor(text_current);
			outtextxy(music_bar_x1 + (music_bar_x2 - music_bar_x1 - textwidth(str_music2)) / 2, music_bar_y1 + 2, str_music2);
		}
		if (!strcmp(str_music, str_music_file3)) {
			setcolor(text_current);
			outtextxy(music_bar_x1 + (music_bar_x2 - music_bar_x1 - textwidth(str_music3)) / 2, music_bar_y1 + 2, str_music3);
		}
		if (!strcmp(str_music, str_music_file4)) {
			setcolor(text_current);
			outtextxy(music_bar_x1 + (music_bar_x2 - music_bar_x1 - textwidth(str_music4)) / 2, music_bar_y1 + 2, str_music4);
		}
		if (!strcmp(str_music, str_music_file5)) {
			setcolor(text_current);
			outtextxy(music_bar_x1 + (music_bar_x2 - music_bar_x1 - textwidth(str_music5)) / 2, music_bar_y1 + 2, str_music5);
		}
		if (flag_music_draw && flag_click && flag_music_click) {
			setcolor(color_background);
			//outtextxy(music_bar_x1 + (music_bar_x2 - music_bar_x1 - textwidth(str_music2)) / 2, music_bar_y1 + 2, "                   ");
			setfillstyle(0, color_background);
			floodfill(music_bar_x1 + (music_bar_x2 - music_bar_x1 - textwidth(str_music2)) / 2, music_bar_y1 + 2, cv_text_normal);
			setcolor(cv_text_normal);
		}
		flag_music_click = false;
	}
}