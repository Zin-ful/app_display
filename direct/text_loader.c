#include <stdio.h>
#include <stdlib.h>
#include "text_loader.h"
#include "windower.h"

unsigned char A_font[8] = {
    0x7C,
    0x12,
    0x12,
    0x7C,
    0x12,
    0x12,
    0x12,
    0x00
};



void draw_char(int row, int col, int size_x, int size_y, unsigned char *font_bitmap, unsigned int color) {
    for (int i = 0; i < 8; i++) {  //in the generation file the size of the char is 8 width
        for (int j = 0; j < 8; j++) {
            if (font_bitmap[i] & (1 << (7 - j))) {
                for (int x = 0; x < size_x; x++) {
                    for (int y = 0; y < size_y; y++) {
                        draw_pixel(row + j * size_x + x, col + i * size_y + y, color);
                    }
                }
            }
        }
    }
}

void print(int start_row, int col, int size_x, int size_y, const char *text, unsigned int color) {
    int curr_row_cell = start_row;
    while (*text){
        draw_char(curr_row_cell, col, size_x, size_y, A_font, color);
        curr_row_cell += 8;
        text++;
    }
}
