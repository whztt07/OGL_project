#include "secondary_functions.h"
#include "Bunches.h"
#include "create_game.h"
#include "Save.h"
#include <Windows.h>
extern Bunches bun;
extern Logic_Nim nim;
extern char str_music[50];
void Continue_game() {
	if (!Loadgame(nim, bun))
		return;
	nim.chosen_bunch = -1;
	nim.chosen_stones = -1;
	PlaySound(NULL, NULL, SND_ASYNC | SND_FILENAME);
	PlaySound(str_music, NULL, SND_ASYNC | SND_FILENAME);
	create_game();
}