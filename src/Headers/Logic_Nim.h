#pragma once

class Logic_Nim
{
private:	
	const int min_num_of_stones = 4;

	
	
public:
	Logic_Nim();
	int choose_bunch();
	int hard_choose_bunch_bot();
	int choose_stones();
	int hard_choose_stones_bot();
	int nim_sum();
	void default_settings();
	int get_min_num_of_stones();
	enum diffic
	{
		easy, hard
	} difficulty;
	enum mode
	{
		bot_vs_player, player1_vs_player2
	} game_mode;
	int chosen_bunch;	// индекс выбранной кучки
	//удаление происходит с конца кучки
	int chosen_stones;	// индекс выбранного камня для удаления в выбранной кучке
	static const int num_of_bunches = 3;
	static const int max_num_of_stones = 10;
	int array_bunches[Logic_Nim::num_of_bunches];
	enum priority
	{
		bot, player, player1, player2
	} who_do_move;
	void set_who_do_move(Logic_Nim::priority who);
	void set_num_chosen_bunch_stone(int num_bunch, int num_stone);
	void set_amount_stones_in_bunches(int num1, int num2, int num3);
	void bot_choosing();
	void man_choosing(int num_of_chosen_bunch, int amount_of_chosen_stones);
	void swap_priority();
	void do_turn();
	bool end_of_game();
	~Logic_Nim();
};