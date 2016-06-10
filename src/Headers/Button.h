#pragma once
#include "Logic_Nim.h"
#include <string>
class Button
{
private:
	struct butt_style
	{
		int color_line;
		int color_norm_fill;
		int color_abnorm_fill;
	};
	struct locat_size
	{
		int left;
		int top;
		int right;
		int bottom;
	};
	struct type_button
	{
		butt_style style;
		locat_size loc_size;
	} exit, pause, makemove, whomove;
	int mx, my;
	bool flag_click;
	bool set_fill_color_and_return_true_if_click(Button::type_button button);
	bool set_bk_color_and_return_true_if_click();
	
public:
	Button();
	char* enum_who_move[4];
	void set_exit_style(int color_line, int color_norm_fill, int color_abnorm_fill);
	void set_exit_loc_size(int left, int top, int sizeX, int sizeY);
	bool draw_exit();

	void set_pause_style(int color_line, int color_norm_fill, int color_abnorm_fill);
	void set_pause_loc_size(int left, int top, int sizeX, int sizeY);
	bool draw_pause();

	void set_makemove_style(int color_line, int color_norm_fill, int color_abnorm_fill);
	void set_makemove_loc_size(int left, int top, int sizeX, int sizeY);
	bool draw_makemove();

	void set_mx_my(int mx, int my);
	void set_flag_click(bool flag);


	~Button();
};

