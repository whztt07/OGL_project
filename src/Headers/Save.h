#pragma once

// ��������� ���������� ����
bool Savedata(int, Logic_Nim::mode &, Logic_Nim::diffic &, Bunches::figure &, int*, Logic_Nim::priority &, char*);
// ��������� �������� ����
bool Loaddata(int, Logic_Nim::mode &, Logic_Nim::diffic &, Bunches::figure &, int*, Logic_Nim::priority &, char*);

// ��������� ������������� ���� ���������� � ��������
void Savegame(Logic_Nim &, Bunches &);
bool Loadgame(Logic_Nim &, Bunches &);

// ��������� ������� �������� ������� ��� ���� ���������� � ��������
void create_snapshot(int, char*);

// ���������� ������� �������� ��������
bool checkdata(int slot, Logic_Nim::mode gamemode, Logic_Nim::diffic difficulty, Bunches::figure form_of_stones, int * stonenumber, Logic_Nim::priority who);
