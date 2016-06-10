#include "Logic_Nim.h"
#include <stdlib.h>
#include <time.h>
#include <memory>

Logic_Nim::Logic_Nim() {
	Logic_Nim::default_settings();
}

void Logic_Nim::default_settings() {
	// иннициализируем некоторые свойства класса
	Logic_Nim::chosen_bunch = -1;
	Logic_Nim::chosen_stones = -1;

	// заполняем кучки случайным количеством камней, таким образом, 
	// чтобы первый, кто делает ход, мог выйграть при безошибочных ходах
	srand(time(0));
	do {
		for (int i = 0; i < Logic_Nim::num_of_bunches; i++)
			Logic_Nim::array_bunches[i] = rand() % (Logic_Nim::max_num_of_stones - Logic_Nim::min_num_of_stones + 1) + Logic_Nim::min_num_of_stones;
	} while (!Logic_Nim::nim_sum());
	// сортируем кучки по возрастанию
	for (int i = 0; i < Logic_Nim::num_of_bunches - 1; i++)
		for (int j = 1; j < Logic_Nim::num_of_bunches; j++)
			if (Logic_Nim::array_bunches[i] > Logic_Nim::array_bunches[j])
				std::swap(Logic_Nim::array_bunches[i], Logic_Nim::array_bunches[j]);
}

int Logic_Nim::get_min_num_of_stones() {
	return Logic_Nim::min_num_of_stones;
}

void Logic_Nim::set_who_do_move(Logic_Nim::priority who) {
	Logic_Nim::who_do_move = who;
}

void Logic_Nim::set_num_chosen_bunch_stone(int num_bunch, int num_stone) {
	Logic_Nim::chosen_bunch = num_bunch;
	Logic_Nim::chosen_stones = num_stone;
}

void Logic_Nim::set_amount_stones_in_bunches(int num1, int num2, int num3) {
	Logic_Nim::array_bunches[0] = num1;
	Logic_Nim::array_bunches[1] = num2;
	Logic_Nim::array_bunches[2] = num3;
}

int Logic_Nim::nim_sum() {
	int sum = Logic_Nim::array_bunches[0];
	for (int i = 1; i < Logic_Nim::num_of_bunches; i++) {
		sum ^= Logic_Nim::array_bunches[i];
	}
	return sum;
}

void Logic_Nim::bot_choosing() {
	Logic_Nim::chosen_bunch = Logic_Nim::choose_bunch();
	Logic_Nim::chosen_stones = Logic_Nim::choose_stones();
}

int Logic_Nim::choose_bunch() {
	int i = 0;
	if (Logic_Nim::difficulty == Logic_Nim::easy)
		do {
			i = rand() % Logic_Nim::num_of_bunches;
		} while (Logic_Nim::array_bunches[i] == 0);
	else
		return Logic_Nim::hard_choose_bunch_bot();
	return i;
}

int Logic_Nim::hard_choose_bunch_bot() {
	if (Logic_Nim::array_bunches[0] > Logic_Nim::array_bunches[1])
		if (Logic_Nim::array_bunches[0] > Logic_Nim::array_bunches[2])
			return 0;
		else
			return 2;
	else
		if (Logic_Nim::array_bunches[1] > Logic_Nim::array_bunches[2])
			return 1;
		else
			return 2;
}

int Logic_Nim::choose_stones(){
	if (Logic_Nim::difficulty == Logic_Nim::easy || Logic_Nim::nim_sum() == 0)
		return rand() % Logic_Nim::array_bunches[Logic_Nim::chosen_bunch];
	else
		return Logic_Nim::hard_choose_stones_bot();
}

int Logic_Nim::hard_choose_stones_bot() {
	int sum_2 = 0;
	for (int i = 0; i < Logic_Nim::num_of_bunches; i++)
		if (i != Logic_Nim::chosen_bunch)
			sum_2 ^= Logic_Nim::array_bunches[i];
	return Logic_Nim::array_bunches[Logic_Nim::chosen_bunch] - abs(Logic_Nim::array_bunches[Logic_Nim::chosen_bunch] - sum_2);
}

void Logic_Nim::man_choosing(int num_of_chosen_bunch, int amount_of_chosen_stones) {
	Logic_Nim::chosen_bunch = num_of_chosen_bunch;
	Logic_Nim::chosen_stones = amount_of_chosen_stones;
}

void Logic_Nim::swap_priority() {
	if (Logic_Nim::game_mode == Logic_Nim::player1_vs_player2) {
		Logic_Nim::who_do_move = (Logic_Nim::priority)((1 - (Logic_Nim::who_do_move - 2)) + 2);
		return;
	}
	Logic_Nim::who_do_move = (Logic_Nim::priority)(1 - Logic_Nim::who_do_move);
}

void Logic_Nim::do_turn() {
	Logic_Nim::array_bunches[Logic_Nim::chosen_bunch] -= Logic_Nim::array_bunches[Logic_Nim::chosen_bunch] - Logic_Nim::chosen_stones;
}

bool Logic_Nim::end_of_game() {
	for (int i = 0; i < Logic_Nim::num_of_bunches; i++)
		if (Logic_Nim::array_bunches[i] > 0)
			return false;
	return true;
}

Logic_Nim::~Logic_Nim(){}
