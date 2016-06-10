#pragma once

// Процедура сохранения игры
bool Savedata(int, Logic_Nim::mode &, Logic_Nim::diffic &, Bunches::figure &, int*, Logic_Nim::priority &, char*);
// Процедура загрузки игры
bool Loaddata(int, Logic_Nim::mode &, Logic_Nim::diffic &, Bunches::figure &, int*, Logic_Nim::priority &, char*);

// Процедуры вырисовывания меню сохранения и загрузки
void Savegame(Logic_Nim &, Bunches &);
bool Loadgame(Logic_Nim &, Bunches &);

// Служебная функция создания пунктов для меню сохранения и загрузки
void create_snapshot(int, char*);

// Внутренняя функция проверки значений
bool checkdata(int slot, Logic_Nim::mode gamemode, Logic_Nim::diffic difficulty, Bunches::figure form_of_stones, int * stonenumber, Logic_Nim::priority who);
