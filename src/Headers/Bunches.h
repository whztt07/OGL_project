#pragma once
#include "Logic_Nim.h"

class Bunches
{
private:	
	int color_line_figure;
	int color_norm_figure;
	int color_abnorm_figure;
	int color_chosen_stones;
	int indent_left_right;
	int indent_top_bottom;
	int size_rad;
	struct coord
	{
		int x;
		int y;
	};
	coord bun_st[Logic_Nim::num_of_bunches][Logic_Nim::max_num_of_stones];
	int coord_lineY[Logic_Nim::num_of_bunches + 1];
	int coord_lineX[Logic_Nim::max_num_of_stones + 1];
	int sizeX_place_fig;
	bool click;

public:
	
	Bunches();
	int color_background;
	int color_line_area;
	enum figure
	{
		trian, cir, rec
	}form;	// выбранная форма фигур

	void draw_razm();
	void draw_all(Logic_Nim & nim);

	void draw_bunch(int number_of_bunch, Logic_Nim & nim);

	bool ismousehere(int num_of_bunch, Logic_Nim & nim);

	int compute_number_stone(int num_bun, Logic_Nim & nim);

	int find_num_st_nearest_for_biginning(int n, Logic_Nim & nim);

	void delete_chosing_stones(Logic_Nim & nim);

	void draw_figure(int num_of_bunch, int num_of_stones);

	void draw_triangle(int rx, int ry);

	void draw_circle(int rx, int ry);

	void draw_rectangle(int rx, int ry);


	void set_mouse_click(bool flag);

	void clear_fild(Logic_Nim &nim);


	~Bunches();
};

