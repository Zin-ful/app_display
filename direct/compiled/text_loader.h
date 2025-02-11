#ifndef TEXT_LOADER_H
#define TEXT_LOADER_H

void draw_char(int row, int col, int size_x, int size_y, unsigned char *font_bitmap, unsigned int color);
void print(int start_row, int col, int size_x, int size_y, const char *text, unsigned int color);

#endif
