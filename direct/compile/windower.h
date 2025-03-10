#ifndef WINDOWS_H
#define WINDOWS_H

#include <stdint.h>

extern signed int screen_res_y;
extern signed int screen_res_x;
extern int cell_width;
extern int cell_height;
extern int text_cell_width;
extern int text_cell_height;
extern int disable_check_app_host;
extern int disable_exit_button;
extern int disable_top_bar;
extern int disable_bottom_bar;
extern int disable_left_bar;
extern int disable_right_bar;
extern int disable_draw_icon;
extern int disable_center_exit;
extern int border_state;
extern int border_1_thickness;
extern int border_2_thickness;
extern int border_3_thickness;
extern int border_4_thickness;

int init_framebuffer();
void close_framebuffer();
void clear_screen();
void screen_event(int grid_pos_x, int grid_pos_y, int mouse_x, int mouse_y);
uint32_t get_pixel(int x, int y);
uint32_t get_pixel_range(int size, uint32_t color_cache[10][10]);
void draw_pixel(int x, int y, uint32_t color);
void erase_pixel(int x, int y, uint32_t color);
void draw_mouse(int x, int y);
void erase_mouse(int x, int y);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void erase_rect(int x, int y, int w, int h);
void draw_highlight(int x, int y, int w, int h, uint32_t color);
void init_apps();
void check_apps(int x, int y);
void draw_top_bar(int w, int h, uint32_t color);
void draw_bottom_bar(int w, int h, uint32_t color);
void draw_left_bar(int w, int h, uint32_t color);
void draw_right_bar(int w, int h, uint32_t color);
void draw_window(int w_1, int w_2, int w_3, int w_4, int h_1, int h_2, int h_3, int h_4, uint32_t color_1, uint32_t color_2, uint32_t color_3, uint32_t color_4, int win_preset, int anim_preset);
#endif
