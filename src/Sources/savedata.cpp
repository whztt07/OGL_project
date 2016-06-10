#include <fstream>
#include <Windows.h>
#include <ctime>
#include <iostream>
#include <string>
#include <cassert> 
#include "Logic_Nim.h"
#include "Bunches.h"
#include "Save.h"

using namespace std;

bool Savedata(int slot, Logic_Nim::mode &gamemode, Logic_Nim::diffic &difficulty, Bunches::figure &form_of_stones, int* stonenumber, Logic_Nim::priority &who, char* music){ 
		if (!checkdata(slot, gamemode, difficulty, form_of_stones, stonenumber, who))
			return false;
        ofstream save;  // ��������� �� ��������� ����������
		time_t date_now;
		time(&date_now);
		string date = ctime(&date_now);  // �������� ������, ���������� ��������������� ���� � ��������� ������
		CreateDirectory("save", NULL);  // �������� ����� � ������������, ���� ��� ��� �� �������
        date[date.length()-1]='\0';  // ������� ������ ����� ������
        switch (slot){
               case 0: save.open("save\\slot1.save"); break;  // �������� ��� ���������� ������� ����� ����������
               case 1: save.open("save\\slot2.save"); break;         
               case 2: save.open("save\\slot3.save"); break;             
               case 3: save.open("save\\slot4.save"); break;              
               case 4: save.open("save\\slot5.save"); break;              
        }
		if (!save.is_open())
			return false;
        save<<date<<" "<<gamemode<<" "<<who<<" "<<difficulty<<" "<<form_of_stones
			<<" "<<stonenumber[0]<<" "<<stonenumber[1]<<" "<<stonenumber[2]<<" "<<music;  // ������ � ���� ������ ����������
	    save.close();  // �������� ����� ����������
	    return true;
}
