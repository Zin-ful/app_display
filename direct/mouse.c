#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include "mouse.h"
#include "windower.h"

#define MOUSE_PATH "/dev/input/mice"

uint32_t cache_color[10][10];
int mouse_fd = -1;
int mouse_pos_x, mouse_pos_y, cache_mouse_x, cache_mouse_y, left_click, right_click, middle_click;

int disable_click_host = 0;

/*
*/


int init_mouse() {
    mouse_fd = open(MOUSE_PATH, O_RDONLY);
    if (mouse_fd == -1) {
        perror("failed to open mouse event\n");
        return -1;
    }
    mouse_pos_y = screen_res_y / 2;
    mouse_pos_x = screen_res_x / 2;
    return 0;
}

void read_mouse() {
    unsigned char data[3];
    while (read(mouse_fd, data, sizeof(data)) > 0) {
        left_click = data[0] & 0x1;
        right_click = data[0] & 0x2; //mouse states
        middle_click = data[0] & 0x4;
        mouse_pos_x += (signed char)data[1];
        mouse_pos_y -= (signed char)data[2];
        //mouse drawing functions in windower.c
        if (left_click == 1) {
            click(mouse_pos_x, mouse_pos_y, left_click);
        } else if (right_click == 2) {
            click(mouse_pos_x, mouse_pos_y, right_click); //make into switch
        } else if (middle_click == 4) {
            click(mouse_pos_x, mouse_pos_y, middle_click);
        }
        if (mouse_pos_x < 0) {
            mouse_pos_x = 4;
        } else if (mouse_pos_x >= screen_res_x) {
            mouse_pos_x = screen_res_x - 4;
        }
        if (mouse_pos_y < 0) {
            mouse_pos_y = 4;
        } else if (mouse_pos_y >= screen_res_y) {
            mouse_pos_y = screen_res_y - 4;
        }
        erase_mouse(cache_mouse_x, cache_mouse_y);
        draw_mouse(mouse_pos_x, mouse_pos_y);
        cache_mouse_x = mouse_pos_x;
        cache_mouse_y = mouse_pos_y;
        //printf("Mouse: Left[%d] Right[%d] Middle[%d] Pos X[%d] Pos Y[%d]\n", left, right, middle, mouse_pos_x, mouse_pos_y);
        usleep(10000);
    }
}

void click(int x, int y, int mouse_click_state) {
    unsigned char cache_data[3];
    int press = 0;
    if (disable_click_host) return;
    switch (mouse_click_state) {
        case 1:
            if (mouse_click_state == 1){
                int cell_x = x / cell_width;
                int cell_y = y / cell_height;
                screen_event(cell_x, cell_y, x, y);
                while (read(mouse_fd, cache_data, sizeof(cache_data)) > 0) {
                    press = cache_data[0] & 0x1;
                    if (press == 0){
                        break;
                    }
                }
            } else {
                break;
            }
        case 2:
            if (mouse_click_state == 2){
                while (read(mouse_fd, cache_data, sizeof(cache_data)) > 0) {
                    press = cache_data[0] & 0x22;
                    if (press == 0){
                        break;
                    }
                }
            } else {
                break;
            }
        case 4:
            if (mouse_click_state == 4){
                while (read(mouse_fd, cache_data, sizeof(cache_data)) > 0) {
                    press = cache_data[0] & 0x4;
                    if (press == 0){
                        break;
                    }
                }
            } else {
                break;
            }
    }
}

void close_mouse() {
    if (mouse_fd != -1) close(mouse_fd);
}
