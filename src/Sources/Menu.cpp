#include "Menu.h"
#include "graphics.h"
#include <string.h>
#include <cmath>

extern int X_max;
extern int Y_max;

#define forestGreen RGB (55, 205, 55)


// Координатой(ами) для строки может(гут) являться либо только координата верхнего левого угла
// либо координаты верхнего левого угла и нижнего правого


Menu::Menu(char* header_name, char** name_body_of_menu, int num_of_strmenu) {
	Menu::num_of_item_menu = num_of_strmenu;
	// задаём стиль текста и цвет фона по умолчанию
	Menu::headline = {BLACK, BLACK, 8, 10};
	Menu::body = {GREEN, YELLOW,  5, 10};
	Menu::color_background = LIGHTBLUE;
	
	// выделим память и инициализируем name_header
	Menu::name_header = new char[strlen(header_name) + 2];
	strcpy(Menu::name_header, header_name);

	// устанавливаем значения по умолчанию для координат вывода заголовка(левый верхний угол)
	// вызываю функцию settextstyle для того, чтобы верно сработала функция textwidht
	settextstyle(Menu::headline.font_text, 0, Menu::headline.char_size);
	Menu::coord_header.x = (X_max - textwidth(header_name)) / 2;
	Menu::coord_header.y = 20;

	if (Menu::num_of_item_menu == 0)
		return;

	// выделяем память по массив координат строк меню
	array_location_of_strs = new location[num_of_item_menu];

	
	
	// выделим память под массив строк(пунктов меню) и заполним его
	Menu::name_body = new char*[Menu::num_of_item_menu];
	for (int i = 0; i < Menu::num_of_item_menu; i++) {
		Menu::name_body[i] = new char[strlen(name_body_of_menu[i]) + 2];
		strcpy(Menu::name_body[i], name_body_of_menu[i]);
	}

	

	// устанавливаем значения координат по умолчанию для поля body_of_menu(прямоугольник вокруг строчек меню)
	// и вычисляем начальные координаты строчек в меню
	// вызываю функцию settextstyle для того, чтобы верно сработала функция textheight в дальнейшей
	// работе функции set_starting_settings_for_output_of_body
	settextstyle(Menu::body.font_text, 0, Menu::body.char_size);
	Menu::set_starting_settings_for_output_of_body(150, 140, 650, 500);
}

void Menu::set_color_background(int color_bkgd) {
	Menu::color_background = color_bkgd;
}
void Menu::draw_background() {
	setfillstyle(1, Menu::color_background);
	bar(0, 0, X_max, Y_max);
}


void Menu::init_array_with_coord_menu() {
	int buff_Y_for_str = Menu::loc_of_body.left_top.y + 2;
	Menu::comp_interval_between_str(textheight("Привет"));
	for (int i = 0; i < Menu::num_of_item_menu; i++) 
		Menu::array_location_of_strs[i] = Menu::compute_coord_str_in_menu(Menu::name_body[i], buff_Y_for_str);
}
void Menu::comp_interval_between_str(int height_str) {
	int free_placeY = Menu::loc_of_body.right_bottom.y - Menu::loc_of_body.left_top.y;
	int temp = (free_placeY - Menu::num_of_item_menu * height_str) / Menu::num_of_item_menu;
	Menu::interval_between_str = temp;
}
Menu::location Menu::compute_coord_str_in_menu(char* str_menu, int &buff) {
	Menu::location coor_str_in_menu;
	coor_str_in_menu.left_top = { Menu::compute_coord_X_str(str_menu), buff};
	coor_str_in_menu.right_bottom = Menu::compute_right_bottom(str_menu, coor_str_in_menu.left_top);
	buff += Menu::interval_between_str + textheight(str_menu);
	return coor_str_in_menu;
}
int Menu::compute_coord_X_str(char* str) {
	int temp = Menu::loc_of_body.right_bottom.x - Menu::loc_of_body.left_top.x;
	return (temp - textwidth(str)) / 2 + Menu::loc_of_body.left_top.x;
}
void Menu::set_style_headline(int color_norm, int char_size, int font_text) {
	Menu::headline.color_norm = color_norm;
	Menu::headline.char_size = char_size;
	Menu::headline.font_text = font_text;
	settextstyle(Menu::headline.font_text, 0, Menu::headline.char_size);
}
void Menu::set_coord_start_headline(int x, int y) {
	Menu::coord_header = { x, y };
}
void Menu::draw_headline() {
	setcolor(Menu::headline.color_norm);
	setbkcolor(Menu::color_background);
	outtextxy(Menu::coord_header.x, Menu::coord_header.y, Menu::name_header);
}

void Menu::set_style_body(int color_norm, int color_abnorm, int char_size, int font_text){
	Menu::body.color_norm = color_norm;
	Menu::body.color_abnorm = color_abnorm;
	Menu::body.char_size = char_size;
	Menu::body.font_text = font_text;
	settextstyle(Menu::body.font_text, 0, Menu::body.char_size);
	setbkcolor(Menu::color_background);
}
void Menu::set_style_body() {
	settextstyle(Menu::body.font_text, 0, Menu::body.char_size);
	setbkcolor(Menu::color_background);
}
void Menu::set_starting_settings_for_output_of_body(int left, int top, int right, int bottom) {
	Menu::loc_of_body.left_top.x = left;
	Menu::loc_of_body.left_top.y = top;
	Menu::loc_of_body.right_bottom.x = right;
	Menu::loc_of_body.right_bottom.y = bottom;
	Menu::init_array_with_coord_menu();
}

void Menu::menu_set_settings_and_draw_background_and_header() {
	Menu::set_color_background(COLOR(167, 252, 0));
	Menu::draw_background();
	Menu::set_style_headline(COLOR(79, 79, 79), 7, 10);
	Menu::set_coord_start_headline((X_max - textwidth(Menu::name_header)) / 2, 30);
	Menu::draw_headline();
	Menu::set_style_body(forestGreen, COLOR(32, 125, 32), 5, 10);
	Menu::set_starting_settings_for_output_of_body(0, 150, X_max, Y_max - 100);
}
void Menu::submenu_set_settings_and_draw_background_and_header() {
	Menu::set_color_background(COLOR(52, 201, 36));
	Menu::draw_background();
	Menu::set_style_headline(COLOR(255, 255, 51), 5, 10);
	Menu::set_coord_start_headline((X_max - textwidth(Menu::name_header)) / 2, 30);
	Menu::draw_headline();
	if (Menu::num_of_item_menu == 0)
		return;
	Menu::set_style_body(COLOR(77, 77, 77), BLACK, 4, 10);
	Menu::set_starting_settings_for_output_of_body(0, 150, X_max, Y_max - 150);
}
int Menu::draw_body(int mx, int my, bool flag_click) {
	for (int i = 0; i < Menu::num_of_item_menu; i++) {
		setcolor(Menu::determine_color_text(mx, my, i));
		outtextxy(Menu::array_location_of_strs[i].left_top.x, Menu::array_location_of_strs[i].left_top.y, Menu::name_body[i]);
		if (if_in_area(array_location_of_strs[i], mx, my))
			if(flag_click)
				return i;
	}
	return -1;
}


Menu::coordinate Menu::compute_right_bottom(char *str, Menu::coordinate left_top) {
	Menu::coordinate temp = { left_top.x + textwidth(str), left_top.y + textheight(str) };
	return temp;
}

bool Menu::if_in_area(Menu::location pos_str, int mx, int my) {
	if ((mx > pos_str.left_top.x) && (mx < pos_str.right_bottom.x) &&
		(my > pos_str.left_top.y) && (my < pos_str.right_bottom.y))
		return true;
	return false;
}

int Menu::determine_color_text(int mx, int my, int i) {
	if (Menu::if_in_area(Menu::array_location_of_strs[i], mx, my))
		return Menu::body.color_abnorm;
	else
		return Menu::body.color_norm;
}

Menu::~Menu() {
	delete [] Menu::name_header;
	if (Menu::num_of_item_menu == 0)
		return;
	for (int i = 0; i < Menu::num_of_item_menu; i++) {
		delete [] Menu::name_body[i];
	}
	delete [] Menu::name_body;
	delete[] Menu::array_location_of_strs;
}
