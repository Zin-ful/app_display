#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include "mouse.h"
#include "windower.h"

#define MOUSE_PATH "/dev/input/mice"

uint32_t cache_color[5][5];
int mouse_fd = -1;
int mouse_pos_x;
int mouse_pos_y;

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
        erase_mouse(1, cache_color);
        int left = data[0] & 0x1;
        int right = data[0] & 0x2; //mouse states
        int middle = data[0] & 0x4;
        mouse_pos_x += (signed char)data[1];
        mouse_pos_y -= (signed char)data[2];
        get_space(mouse_pos_x, mouse_pos_y, 1, cache_color);
        draw_mouse(mouse_pos_x, mouse_pos_y, 1, 0xFFFFFF);
        usleep(20000);
        if (left == 1) {
            click(mouse_pos_x, mouse_pos_y, left);
        } else if (right == 2) {
            click(mouse_pos_x, mouse_pos_y, right);
        } else if (middle == 4) {
            click(mouse_pos_x, mouse_pos_y, middle);
        }
        if (mouse_pos_x < 0) {
            mouse_pos_x = 1;
        } else if (mouse_pos_x >= screen_res_x) {
            mouse_pos_x = screen_res_x - 1;
        }
        if (mouse_pos_y < 0) {
            mouse_pos_y = 1;
        } else if (mouse_pos_y >= screen_res_y) {
            mouse_pos_y = screen_res_y - 1;
        }
        //printf("Mouse: Left[%d] Right[%d] Middle[%d] Pos X[%d] Pos Y[%d]\n", left, right, middle, mouse_pos_x, mouse_pos_y);
    }
}

void click(int x, int y, int mouse_state) {
    unsigned char cache_data[3];
    int press = 0;
    switch (mouse_state) {
        case 1:
            if (mouse_state == 1){
                draw_rect(x - 4, y - 4, 8, 8, 0xFFFFFF); //4 is offset to keep it center of mouse kinda
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
            if (mouse_state == 2){
                draw_rect(x - 4, y - 4, 8, 8, 0xFF6666); //4 is offset to keep it center of mouse kinda
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
            if (mouse_state == 4){
                draw_rect(x - 4, y - 4, 8, 8, 0xFF9999); //4 is offset to keep it center of mouse kinda
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
    erase_rect(x - 4, y - 4, 8, 8);
    draw_pixel(x, y, 0xFFFFFF);
}

void close_mouse() {
    if (mouse_fd != -1) close(mouse_fd);
}
