#ifndef MOUSE_H
#define MOUSE_H

extern int mouse_pos_x;
extern int mouse_pos_y;
extern int left_click;
extern int right_click;
extern int middle_click;
extern int disable_click_host;

int init_mouse();
void read_mouse();
void close_mouse();
void click(int x, int y, int mouse_state);
#endif
