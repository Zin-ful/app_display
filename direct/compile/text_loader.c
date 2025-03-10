#include <stdio.h>
#include <stdlib.h>
#include "text_loader.h"
#include "windower.h"
#include <linux/input.h>

int cursor_x, cursor_y;

unsigned char all_fonts[26][8] = {
    //a
    {0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00},
    //b
    {0x7C, 0x42, 0x42, 0x7C, 0x42, 0x42, 0x7C, 0x00},
    //c
    {0x3C, 0x42, 0x40, 0x40, 0x40, 0x42, 0x3C, 0x00},
    //d
    {0x78, 0x44, 0x42, 0x42, 0x42, 0x44, 0x78, 0x00},
    //e
    {0x7E, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x7E, 0x00},
    //f
    {0x7E, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x00},
    //g
    {0x3C, 0x42, 0x40, 0x4E, 0x42, 0x42, 0x3C, 0x00},
    //h
    {0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00},
    //i
    {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00},
    //j
    {0x1E, 0x04, 0x04, 0x04, 0x04, 0x44, 0x38, 0x00},
    //k
    {0x42, 0x44, 0x48, 0x70, 0x48, 0x44, 0x42, 0x00},
    //l
    {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7E, 0x00},
    //m
    {0x42, 0x66, 0x5A, 0x42, 0x42, 0x42, 0x42, 0x00},
    //n
    {0x42, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x42, 0x00},
    //o
    {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00},
    //p
    {0x7C, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40, 0x00},
    //q
    {0x3C, 0x42, 0x42, 0x42, 0x4A, 0x44, 0x3A, 0x00},
    //r
    {0x7C, 0x42, 0x42, 0x7C, 0x48, 0x44, 0x42, 0x00},
    //s
    {0x3C, 0x42, 0x40, 0x3C, 0x02, 0x42, 0x3C, 0x00},
    //t
    {0x7F, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00},
    //u
    {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00},
    //v
    {0x42, 0x42, 0x42, 0x42, 0x24, 0x24, 0x18, 0x00},
    //w
    {0x42, 0x42, 0x42, 0x42, 0x5A, 0x66, 0x42, 0x00},
    //x
    {0x42, 0x24, 0x18, 0x18, 0x18, 0x24, 0x42, 0x00},
    //y
    {0x42, 0x24, 0x18, 0x08, 0x08, 0x08, 0x08, 0x00},
    //z
    {0x7E, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7E, 0x00},
};
unsigned char space_bitmap[8] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char backspace_bitmap[8] = {
 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x00
};

const char *keycode_to_char[256] = {
[KEY_A] = "A", [KEY_B] = "B", [KEY_C] = "C", [KEY_D] = "D", [KEY_E] = "E", [KEY_F] = "F",
[KEY_G] = "G", [KEY_H] = "H", [KEY_I] = "I", [KEY_J] = "J", [KEY_K] = "K", [KEY_L] = "L",
[KEY_M] = "M", [KEY_N] = "N", [KEY_O] = "O", [KEY_P] = "P", [KEY_Q] = "Q", [KEY_R] = "R",
[KEY_S] = "S", [KEY_T] = "T", [KEY_U] = "U", [KEY_V] = "V", [KEY_W] = "W", [KEY_X] = "X",
[KEY_Y] = "Y", [KEY_Z] = "Z", [KEY_SPACE] = " ", [KEY_BACKSPACE] = "\b", [KEY_ENTER] = "\n",
};

void draw_char(int col, int row, int size_x, int size_y, unsigned char *font_bitmap, unsigned int color) { //col and row are swapped to change the orientation of printing. idk why its just fucked
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (font_bitmap[i] & (1 << (7 - j))) {
                for (int x = 0; x < size_x; x++) {
                    for (int y = 0; y < size_y; y++) {
                        draw_pixel(col + j * size_x + x, row + i * size_y + y, color);
                    }
                }
            }
        }
    }
}

void print(int cell_width, int cell_height, int keycode, unsigned int color) {
    unsigned char *bitmap = NULL;
    const char *keybit = keycode_to_char[keycode];
    if (!keybit) return;
    if (cursor_x / text_cell_width >= screen_res_x / text_cell_width - border_2_thickness) {
        cursor_y += text_cell_height;
        cursor_x = border_4_thickness;
    }

    if (*keybit >= 'A' && *keybit <= 'Z') {
        bitmap = all_fonts[*keybit - 'A'];
    } else if (*keybit == " ") {
        bitmap = space_bitmap;
    } else if (keycode == KEY_BACKSPACE) {
        if (cursor_x <= border_4_thickness && cursor_y <= border_1_thickness) return;
        bitmap = backspace_bitmap;
        color = 0x000000;
        if (cursor_x <= border_4_thickness) {
           cursor_y -= text_cell_height;
           cursor_x = screen_res_x - text_cell_width * 2;
        } else {
            cursor_x -= cell_width;
        }
    } else if (keycode == KEY_ENTER) {
        cursor_y += text_cell_height;
        cursor_x = border_4_thickness;
    }
    if (bitmap) {
        draw_char(cursor_x, cursor_y, cell_width / 8, cell_height / 8, bitmap, color);
    }
    if (keycode != KEY_ENTER && keycode != KEY_BACKSPACE) {
        cursor_x += text_cell_width;
    }
}


void printout(int cell_width, int cell_height, char *string, unsigned int color) {
    unsigned char *bitmap = NULL;
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] >= 'A' && string[i] <= 'Z') {
            bitmap = all_fonts[string[i] - 'A'];
        } else if (string[i] == ' ') {
            bitmap = space_bitmap;
        } else if (string[i] == '\n' || string[i] == '\0') {
            bitmap = space_bitmap;
            cursor_y += cell_height;
            cursor_x = border_4_thickness;
        }
        if (bitmap) {
            draw_char(cursor_x, cursor_y, cell_width / 8, cell_height / 8, bitmap, color);
        }
        if (string[i] != '\n') {
            cursor_x += cell_width;
        }
    }
    cursor_y += cell_height;
    cursor_x = border_4_thickness;
}
