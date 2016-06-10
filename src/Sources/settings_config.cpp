#include "Bunches.h"
#include <fstream>
#include <cstring>

using namespace std;
char str_music1[] = "Crossroads"; // массивы с названием музыки
char str_music2[] = "Lonely Girl";
char str_music3[] = "Nocturne";
char str_music4[] = "Sleep away";
char str_music5[] = "Stoltzman";
char str_music_file1[] = "Crossroads.wav";
char str_music_file2[] = "Lonely_Girl.wav";
char str_music_file3[] = "Nocturne.wav";
char str_music_file4[] = "Sleep_Away.wav";
char str_music_file5[] = "Stoltzman.wav";

// создаёт и записывает данные в config - файл
void put_settings(int form, char str_music[50]) {
	ofstream config_file_out;
	char buf1[] = "Строка 1";
	char buf2[] = "Строка 2";
	config_file_out.open("config.cfg", ios_base::out);
	config_file_out << "// 0 = triangle, 1 = circle, 2 = square\n";
	config_file_out << "FORM= " << form << "\n";
	config_file_out << "// " << str_music_file1 << ", " << str_music_file2 << ", " << str_music_file3 << ", " << str_music_file4 << ", " << str_music_file5 << "\n";
	config_file_out << "MUSIC= ";
	config_file_out << str_music;
	config_file_out.close();
}

// читает данные из config - файла
void get_settings(Bunches &bun, char *str_music) {
	int str_number = 50; // колличество строк, которые будут считаны из config - файла
	int str_symbol = 1000; // максимальное колличество символов в каждой строке
	char * buf = new char[str_symbol]; // буфер для хранения данных, считанных из config - файла
	bool flag_form_stones = false; // показывает, найдена ли запись "FORM= ***" в config - файле
	bool flag_music = false; // показывает, найдена ли запись "MUSIC= ***" в config - файле
	ifstream config_file_in;
	config_file_in.open("config.cfg", ios_base::in);
	if (!config_file_in.is_open()) {
		put_settings(0, str_music_file4);
		get_settings(bun, str_music);
	} else {
		for (int i = 0; i < str_number; i++) {
			if (config_file_in.eof())
				break;
			config_file_in >> buf;
			if (!strncmp(buf, "FORM=", 5)) {
				config_file_in >> buf;
				if (buf[0] == 50)
					bun.form = Bunches::figure::rec;
				if (buf[0] == 49)
					bun.form = Bunches::figure::cir;
				if (buf[0] == 48)
					bun.form = Bunches::figure::trian;
				flag_form_stones = true;
			}
			if (!strncmp(buf, "MUSIC=", 6)) {
				config_file_in >> buf;
				strcpy(str_music, buf);
				if (strcmp(str_music, str_music_file1))
					if (strcmp(str_music, str_music_file2))
						if (strcmp(str_music, str_music_file3))
							if (strcmp(str_music, str_music_file4))
								if (strcmp(str_music, str_music_file5)) {
									put_settings(bun.form, str_music_file4);
									get_settings(bun, str_music);
								}
				flag_music = true;
			}
			if (flag_form_stones && flag_music)
				break;
		}
		if (!flag_form_stones && !flag_music)
			put_settings(0, str_music_file4);
		if (!flag_form_stones && flag_music)
			put_settings(0, str_music);
		if (flag_form_stones && !flag_music)
			put_settings(bun.form, str_music_file4);
	}
	delete[] buf;
	config_file_in.close();
}