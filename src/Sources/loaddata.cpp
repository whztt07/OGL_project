#include <fstream>
#include <ctime>
#include <iostream>
#include <string>
#include <cassert> 
#include "Logic_Nim.h"
#include "Bunches.h"
#include "Save.h"

using namespace std;

bool Loaddata(int slot, Logic_Nim::mode &gamemode, Logic_Nim::diffic &difficulty, Bunches::figure &form_of_stones, int* stonenumber, Logic_Nim::priority &who, char *music) {
        string time;  // ������������ �������� ������, ������� ����� "�������" �����
        ifstream save;  // ��������� �� ��������� ����������
		int tmp_mode, tmp_who, tmp_diff, tmp_form; // ��������� ���������� ��� ������ � ���������� ������
        switch (slot){
               case 0: save.open("save\\slot1.save"); break;
               case 1: save.open("save\\slot2.save"); break;         
               case 2: save.open("save\\slot3.save"); break;             
               case 3: save.open("save\\slot4.save"); break;              
               case 4: save.open("save\\slot5.save"); break;
			   default: return false; //���� ��������� �����-�� ������ ��������, ��� ������� ������������
        }
		if (!save.is_open())
			return false;
        save >> time; save >> time; save >> time; save >> time; save >> time;  //���� ������� �� 5 ���������
        save >> tmp_mode;
		gamemode = (Logic_Nim::mode) tmp_mode;
        save >> tmp_who;
		who = (Logic_Nim::priority) tmp_who;
        save >> tmp_diff;
		difficulty = (Logic_Nim::diffic) tmp_diff;
        save >> tmp_form;
		form_of_stones = (Bunches::figure) tmp_form;
        save >> stonenumber[0];
        save >> stonenumber[1];
        save >> stonenumber[2];
		save >> music;
        save.close();  // �������� ����� ����������
        if (!checkdata(slot, gamemode, difficulty, form_of_stones, stonenumber, who))
			return false;
		return true;
}
