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
uint32_t get_pixel(int x, int y);
uint32_t get_pixel_range(int x, int y, int size, uint32_t color_cache[5][5]);
uint32_t get_space(int x, int y, int size, uint32_t color_cache[5][5]);
void draw_pixel(int x, int y, uint32_t color);
void erase_pixel(int x, int y, uint32_t color);
void draw_mouse(int x, int y, int size, uint32_t color);
void erase_mouse(int size, uint32_t color_cache[5][5]);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void erase_rect(int x, int y, int w, int h);
void draw_highlight(int x, int y, int w, int h, uint32_t color);
#endif
