
#ifndef TEXT_LOADER_H
#define TEXT_LOADER_H
extern int cursor_x;
extern int cursor_y;

void draw_char(int row, int col, int size_x, int size_y, unsigned char *font_bitmap, unsigned int color);
void print(int cell_width, int cell_height, int keycode, unsigned int color);
void printout(int cell_width, int cell_height, char *string, unsigned int color);

#endif
