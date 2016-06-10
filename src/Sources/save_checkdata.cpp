#include <cassert> 
#include "Logic_Nim.h"
#include "Bunches.h"

bool checkdata(int slot, Logic_Nim::mode gamemode, Logic_Nim::diffic difficulty, Bunches::figure form_of_stones, int* stonenumber, Logic_Nim::priority who) {
	if (slot < 0 || slot > 4)
		return false;
	if (gamemode < 0 || gamemode > 1)
		return false;
	if (difficulty < 0 || difficulty > 1)
		return false;
	if (who < 0 || who > 3)
		return false;
	if (form_of_stones < 0 || form_of_stones > 2)
		return false;
	if (stonenumber[0] < 0 || stonenumber[0] > Logic_Nim::max_num_of_stones)
		return false;
	if (stonenumber[1] < 0 || stonenumber[1] > Logic_Nim::max_num_of_stones)
		return false;
	if (stonenumber[2] < 0 || stonenumber[2] > Logic_Nim::max_num_of_stones)
		return false;
	return true;
}