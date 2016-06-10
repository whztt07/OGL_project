#include <fstream>

using namespace std;

void create_snapshot(int digit, char* slot) {
	ifstream save;
	char num[2];
	itoa(digit+1, num, 10); // ����� � �������� ���������� � 1, ������� digit+1 
	const short str_size = 17; // strlen("save\\slotN.save")=16+1 
	char fname[str_size];
	strcpy(fname, "save\\slot");
	strcat(fname, num);
	strcat(fname, ".save");
	char buff[50];  // ������ ��� �������� ���� � ������� (�������� � ��������� ��������� ����)
	save.open(fname);
	if (save) {  // ���� ���� ����������
		strcpy(slot, "Save ");
		strcat(slot, num);
		strcat(slot, ":");
		for (int i = 0; i<5; i++) {  // ���� ������� �� 5 ����������� ���������: ��� ������, ������, �����, ������� � ����
			save >> buff;
			strcat(slot, buff);
			strcat(slot, " ");
		}
		strcat(slot, "\0");
	}
	else strcpy(slot, "-////-");
	save.close();
}