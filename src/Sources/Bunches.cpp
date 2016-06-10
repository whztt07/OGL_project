#include "Bunches.h"
#include "graphics.h"
#include <iostream>
extern int X_max;
extern int Y_max;
Bunches::Bunches() {
	Bunches::color_background = COLOR(167, 200, 0);
	Bunches::color_line_area = BLACK;
	Bunches::color_line_figure = BLUE;
	Bunches::color_norm_figure = COLOR(125, 127, 125);
	Bunches::color_abnorm_figure = COLOR(190, 153, 80);
	Bunches::color_chosen_stones = COLOR(54, 54, 54);
	Bunches::indent_left_right = 50;
	Bunches::indent_top_bottom = 100;
	Bunches::size_rad = 30;
	Bunches::click = false;
}

void Bunches::draw_razm() {
	setlinestyle(0, 0, 1);
	setcolor(Bunches::color_line_area);
	rectangle(Bunches::indent_left_right, Bunches::indent_top_bottom, X_max - Bunches::indent_left_right, Y_max - Bunches::indent_top_bottom);
	// рисуем вертикальные линии
	int indent = (Y_max - 2 * Bunches::indent_top_bottom) / Logic_Nim::num_of_bunches;
	int buff = Bunches::indent_top_bottom;
	for (int i = 0; i < Logic_Nim::num_of_bunches; i++, buff += indent) {
		Bunches::coord_lineY[i] = buff;
		line(Bunches::indent_left_right, buff, X_max - Bunches::indent_left_right, buff);
		for (int j = 0; j < Logic_Nim::max_num_of_stones; j++)
			Bunches::bun_st[i][j].y = buff + indent / 2;
	}
	Bunches::coord_lineY[Logic_Nim::num_of_bunches] = buff;
	// рисуем горизонтальные линии
	indent = (X_max - 2 * Bunches::indent_left_right) / Logic_Nim::max_num_of_stones;
	Bunches::sizeX_place_fig = indent;
	buff = Bunches::indent_left_right;
	for (int i = 0; i < Logic_Nim::max_num_of_stones; i++, buff += indent) {
		Bunches::coord_lineX[i] = buff;
		line(buff, Bunches::indent_top_bottom, buff, Y_max - Bunches::indent_top_bottom);
		for (int j = 0; j < Logic_Nim::num_of_bunches; j++)
			Bunches::bun_st[j][i].x = buff + indent / 2;
	}
	Bunches::coord_lineX[Logic_Nim::max_num_of_stones] = buff;
}

void Bunches::draw_all(Logic_Nim &nim) {
	setlinestyle(0, 0, 1);
	for (int i = 0; i < Logic_Nim::num_of_bunches; i++)
		Bunches::draw_bunch(i, nim);
}

void Bunches::draw_bunch(int number_of_bunch, Logic_Nim &nim) {
	int n = Bunches::compute_number_stone(number_of_bunch, nim);
	if (number_of_bunch == nim.chosen_bunch) {
		if (ismousehere(number_of_bunch, nim)) {
			if (Bunches::click) {
				if (n == nim.chosen_stones)
					nim.man_choosing(-1, -1);
				else
					nim.man_choosing(number_of_bunch, n);
				return;
			}
			int n1 = Bunches::find_num_st_nearest_for_biginning(n, nim);
			setfillstyle(1, Bunches::color_norm_figure);
			for (int i = 0; i < n1; i++)
				Bunches::draw_figure(number_of_bunch, i);
			setfillstyle(1, Bunches::color_abnorm_figure);
			for (int i = n1; i < nim.chosen_stones; i++)
				Bunches::draw_figure(number_of_bunch, i);
			setfillstyle(1, Bunches::color_chosen_stones);
			for (int i = nim.chosen_stones; i < nim.array_bunches[number_of_bunch]; i++)
				Bunches::draw_figure(number_of_bunch, i);
		}
		else {
			setfillstyle(1, Bunches::color_norm_figure);
			for (int i = 0; i < nim.chosen_stones; i++)
				Bunches::draw_figure(number_of_bunch, i);
			setfillstyle(1, Bunches::color_chosen_stones);
			for (int i = nim.chosen_stones; i < nim.array_bunches[number_of_bunch]; i++)
				Bunches::draw_figure(number_of_bunch, i);
		}
	}
	else {
		if (Bunches::ismousehere(number_of_bunch, nim)) {
			if (Bunches::click) {
				nim.man_choosing(number_of_bunch, n);
				return;
			}
			setfillstyle(1, Bunches::color_norm_figure);
			for (int i = 0; i < n; i++)
				Bunches::draw_figure(number_of_bunch, i);
			setfillstyle(1, Bunches::color_abnorm_figure);
			for (int i = n; i < nim.array_bunches[number_of_bunch]; i++)
				Bunches::draw_figure(number_of_bunch, i);
		}
		else {
			setfillstyle(1, Bunches::color_norm_figure);
			for (int i = 0; i < nim.array_bunches[number_of_bunch]; i++)
				Bunches::draw_figure(number_of_bunch, i);
		}
	}
}

bool Bunches::ismousehere(int num_of_bunch, Logic_Nim &nim) {
	int mx = mousex();
	int my = mousey();
	if (my > Bunches::coord_lineY[num_of_bunch] &&
		my < Bunches::coord_lineY[num_of_bunch + 1] &&
		mx > Bunches::coord_lineX[0] &&
		mx < Bunches::coord_lineX[nim.array_bunches[num_of_bunch]])
		return true;
	return false;
}
int Bunches::compute_number_stone(int num_bun, Logic_Nim &nim) {	
	return (mousex() - Bunches::indent_left_right) / Bunches::sizeX_place_fig;
}
int Bunches::find_num_st_nearest_for_biginning(int n, Logic_Nim &nim) {
	if (n <= nim.chosen_stones)
		return n;
	else
		return nim.chosen_stones;
}

void Bunches::delete_chosing_stones(Logic_Nim &nim) {
	setfillstyle(1, Bunches::color_background);
	for (int i = nim.chosen_stones; i <  nim.array_bunches[nim.chosen_bunch]; i++)
		floodfill(Bunches::bun_st[nim.chosen_bunch][i].x, Bunches::bun_st[nim.chosen_bunch][i].y, Bunches::color_line_area);
}


void Bunches::draw_figure(int num_of_bunch, int num_of_stones) {
	setcolor(Bunches::color_line_figure);
	switch (Bunches::form)
	{
	case Bunches::trian:
		Bunches::draw_triangle(Bunches::bun_st[num_of_bunch][num_of_stones].x, Bunches::bun_st[num_of_bunch][num_of_stones].y);
		break;
	case Bunches::cir:
		Bunches::draw_circle(Bunches::bun_st[num_of_bunch][num_of_stones].x, Bunches::bun_st[num_of_bunch][num_of_stones].y);
		break;
	case Bunches::rec:
		Bunches::draw_rectangle(Bunches::bun_st[num_of_bunch][num_of_stones].x, Bunches::bun_st[num_of_bunch][num_of_stones].y);
		break;
	default:
		break;
	}
}
void Bunches::draw_triangle(int rx, int ry) {
	line(rx, ry - Bunches::size_rad, rx + Bunches::size_rad, ry + Bunches::size_rad);
	line(rx, ry - Bunches::size_rad, rx - Bunches::size_rad, ry + Bunches::size_rad);
	line(rx - Bunches::size_rad, ry + Bunches::size_rad, rx + Bunches::size_rad, ry + Bunches::size_rad);
	floodfill(rx, ry, Bunches::color_line_figure);
}
void Bunches::draw_circle(int rx, int ry) {
	circle(rx, ry, Bunches::size_rad);
	floodfill(rx, ry, Bunches::color_line_figure);
}
void Bunches::draw_rectangle(int rx, int ry) {
	rectangle(rx - Bunches::size_rad, ry - Bunches::size_rad, rx + Bunches::size_rad, ry + Bunches::size_rad);
	floodfill(rx, ry, Bunches::color_line_figure);
}

void Bunches::set_mouse_click(bool flag) {
	Bunches::click = flag;
}

void Bunches::clear_fild(Logic_Nim &nim) {
	setfillstyle(1, Bunches::color_background);
	for (int i = 0; i < nim.num_of_bunches; i++)
		for (int j = 0; j < nim.array_bunches[i]; j++)
			floodfill(Bunches::bun_st[i][j].x, Bunches::bun_st[i][j].y, Bunches::color_line_area);
}

Bunches::~Bunches() {}
