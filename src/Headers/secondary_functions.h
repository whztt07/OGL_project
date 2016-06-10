#pragma once
#include "Logic_Nim.h"
#include "Bunches.h"

void choose_difficulty(Logic_Nim & nim);

bool choose_mode(Logic_Nim & nim);

void draw_who_do_move(Logic_Nim & nim, Bunches &bun, int color_text);

void draw_end(Logic_Nim &nim, Bunches &bun);

bool Pause();

void getstate__func__putstate(void (*function)());

bool getstate__func__putstate(bool(*function)());

