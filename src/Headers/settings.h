// рисует фигуры (0 - triangle, 1 - circle, 2 - square)
void figure_draw(int x, int y, int size, int form);

// опреледяет, находится ли курсор в фигуре (0 - triangle, 1 - circle, 2 - square)
bool figure_is_in_area(int x, int y, int size, int form, int mx, int my);

// рисует стрелку (0 - вверх, 1 - вниз)
void arrow_draw(int x, int y, int width, int height, bool flag);

// определяет, находится ли курсор в тексте
bool Is_in_area(int mx, int my, char* str, int x, int y);

// рисует список музыки
void music_list_draw(int x1, int y1, int x2, int y2, int mx, int my, bool &flag_music_draw, bool &flag_music_click, bool flag_click);

// функция, рисующая меню настроек
void Settings();