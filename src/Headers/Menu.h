#pragma once

class Menu
{
private:

	
	int interval_between_str;

	struct settings_text
	{
		int color_norm;
		int color_abnorm;
		int char_size;
		int font_text;
	} headline, body;
	struct coordinate
	{
		int x;
		int y;
	} coord_header;


	char* name_header;
	char** name_body;


	void init_array_with_coord_menu();
	void comp_interval_between_str(int height_str);
	
	int compute_coord_X_str(char * str);
	Menu::coordinate compute_right_bottom(char * str, Menu::coordinate left_top);

	int determine_color_text(int mx, int my, int i);

public:
	int num_of_item_menu;
	struct location
	{
		coordinate left_top;
		coordinate right_bottom;
	} loc_of_body;
	location *array_location_of_strs;
	bool if_in_area(Menu::location pos_str, int mx, int my);
	Menu::location compute_coord_str_in_menu(char * str_menu, int & buff);
	Menu(char * header_name, char ** name_body_of_menu, int num_of_strmenu);
	int color_background;

	void set_color_background(int color_bkgd);
	void draw_background();

	void set_style_headline(int color_norm, int char_size, int font_text);
	void set_coord_start_headline(int x, int y);
	void draw_headline();

	void set_style_body(int color_norm, int color_abnorm, int char_size, int font_text);
	void set_style_body();
	void set_starting_settings_for_output_of_body(int left, int top, int right, int bottom);

	void menu_set_settings_and_draw_background_and_header();

	void submenu_set_settings_and_draw_background_and_header();
	

	int draw_body(int mx, int my, bool flag_click);

	~Menu();
};