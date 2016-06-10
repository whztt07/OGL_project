#include "secondary_functions.h"
#include "Bunches.h"
#include "create_game.h"
#include "settings_config.h"

extern Bunches bun;
extern Logic_Nim nim;
extern char str_music[50];

void New_game() {
	nim.default_settings();
	get_settings(bun, str_music);
	if(choose_mode(nim))
		choose_difficulty(nim);
	create_game();
}
