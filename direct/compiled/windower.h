#ifndef WINDOWS_H
#define WINDOWS_H

#include <stdint.h>

extern signed int screen_res_y;
extern signed int screen_res_x;
extern int cell_width;
extern int cell_height;
int init_framebuffer();
void close_framebuffer();
void clear_screen();
void screen_event(int grid_pos_x, int grid_pos_y, int mouse_x, int mouse_y);
void draw_pixel(int x, int y, uint32_t color);
void erase_pixel(int x, int y);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void erase_rect(int x, int y, int w, int h);
void draw_highlight(int x, int y, int w, int h, uint32_t color);
#endif
