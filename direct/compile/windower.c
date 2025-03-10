#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include "windower.h"
#include "mouse.h"
#include "keyboard.h"
#include "text_loader.h"
#include <string.h>
#include <signal.h>

int fb_fd = -1;
void* framebuffer = NULL;
struct fb_var_screeninfo vinfo;
signed int screen_res_x;
signed int screen_res_y; //to sync draw/erase/getcolor
int cell_width, cell_height, text_cell_width, text_cell_height, cache_x, cache_y, erase_cache_x, erase_cache_y, cont_x, cont_y, local;
int border_1_thickness, border_2_thickness, border_3_thickness, border_4_thickness;
int cont = 0;
int disable_check_app_host = 0;
int disable_exit_button = 0;
int disable_top_bar = 0;
int disable_bottom_bar = 0;
int disable_right_bar = 0;
int disable_left_bar = 0;
int disable_draw_icon = 1;
int disable_center_exit = 1;
int disable_home_screen = 0;
int border_state = 3; //this will tell text_loader what position to spawn letters. 1 = top and/or bottom borders are active 2 = sides 3 = all 0 = none

#define GRID_ROW_TEXT 200
#define GRID_COL_TEXT 60
#define GRID_ROW_MAIN 10
#define GRID_COL_MAIN 20
#define GRID_ROW_ICON 0
#define GRID_COL_ICON 0
#define BLACK16 0x0000
#define BLACK24 0x00
#define BLACK32 0xFF000000
#define WHITE32 0xFFFFFF
#define MOUSE_SIZE 4

uint32_t cursor[MOUSE_SIZE][MOUSE_SIZE] = {
    {0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF},
    {0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000},
    {0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000},
    {0xFFFFFF, 0x000000, 0x000000, 0x000000},
};

uint32_t background_cache[MOUSE_SIZE][MOUSE_SIZE];

int init_framebuffer() {
    //open fb0
    fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd == -1) {
        perror("failed to open framebuffer\n");
        return -1;
    }

    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("failed reading framebuffer info");
        close(fb_fd);
        return -1;
    }
    screen_res_y = vinfo.yres;
    screen_res_x = vinfo.xres;
    cell_width = screen_res_x / GRID_ROW_MAIN;
    cell_height = screen_res_y / GRID_COL_MAIN; //decrease value to increase size
    text_cell_width = screen_res_x / GRID_ROW_TEXT;
    text_cell_height = screen_res_y / GRID_COL_TEXT;
    cache_x = screen_res_x / 2;
    cache_y = screen_res_y / 2;
    framebuffer = mmap(NULL, vinfo.yres_virtual * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8), PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    clear_screen();
    if (!disable_home_screen) {
        draw_window(screen_res_x, screen_res_y, 8, 8, 8, 8, screen_res_y, screen_res_y, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 1, 0);
        cursor_x = border_4_thickness;
        cursor_y = border_1_thickness;
        init_apps();
    }
    if (framebuffer == MAP_FAILED) {
        perror("Failed to map framebuffer");
        return -1;
    }
    return 0;

}

void close_framebuffer() {
    if (framebuffer != NULL) {
        munmap(framebuffer, vinfo.yres_virtual * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8));
    }
    if (fb_fd == -1) {
        close(fb_fd);
    }
}

void clear_screen() {
    int screensize = vinfo.yres_virtual * vinfo.xres_virtual;
    if (vinfo.bits_per_pixel == 16) {
        uint16_t* fb_ptr = (uint16_t*)framebuffer;
        for (int i = 0; i < screensize; i++) {
            fb_ptr[i] = BLACK16;
        }
    } else if (vinfo.bits_per_pixel == 24) {
        uint8_t* fb_ptr = (uint8_t*)framebuffer;
        for (int i = 0; i < screensize * 3; i += 3) {
            fb_ptr[i] = BLACK24;
            fb_ptr[i + 1] = BLACK24;
            fb_ptr[i + 2] = BLACK24;
        }
    } else if (vinfo.bits_per_pixel == 32) {
        uint32_t* fb_ptr = (uint32_t*)framebuffer;
        for (int i = 0; i < screensize; i++) {
            fb_ptr[i] = BLACK32;
        }
    }
}
void screen_event(int grid_pos_x, int grid_pos_y, int mouse_x, int mouse_y) { //the grid isnt defined here, its defined in mouse.c since the grid is just math
    if (!disable_check_app_host) {
        draw_highlight((grid_pos_x * cell_width), (grid_pos_y * cell_height), cell_width, cell_height, 0xFFFFFF);
        check_apps(grid_pos_x, grid_pos_y);
        if (cont == 1) {
            draw_highlight((grid_pos_x * cell_width), (grid_pos_y * cell_height), cell_width, cell_height, 0x000000);
        }
    }
    if (!disable_exit_button) {
        if (!disable_center_exit) {
            local = screen_res_x / 2 + 8;
        } else {
            local = screen_res_x;
        }
        if (mouse_x <= local && mouse_x >= local - 8 && mouse_y >= 0 && mouse_y <= 8) {
            raise(SIGINT);
        }
    }
}

uint32_t get_pixel(int x, int y) {
    if (x >= vinfo.xres || y >= vinfo.yres) {
        return;
    }
    int index = (y * vinfo.xres_virtual) + x;
    if (vinfo.bits_per_pixel == 16) {
        return ((uint16_t*)framebuffer)[index];
    } else if (vinfo.bits_per_pixel == 24) {
        uint8_t* fb_ptr = (uint8_t*)framebuffer;
        index *= 3;
        return (fb_ptr[index] | (fb_ptr[index + 1] << 8) | (fb_ptr[index + 2] << 16));
    } else if (vinfo.bits_per_pixel == 32) {
        return ((uint32_t*)framebuffer)[index];
    }
}

uint32_t get_pixel_range(int size, uint32_t color_cache[10][10]) {
    for (int dy = 0; dy < size; dy++) {
        for (int dx = 0; dx < size; dx++) {
            color_cache[dy][dx] = get_pixel(erase_cache_x + dx, erase_cache_y + dy);
        }
    }
}

void draw_pixel(int x, int y, uint32_t color) {
    if (x >= vinfo.xres || y >= vinfo.yres) {
        return;
    }
    int index = (y * vinfo.xres_virtual) + x;
    if (vinfo.bits_per_pixel == 16) {
        ((uint16_t*)framebuffer)[index] = (uint16_t)color;
    } else if (vinfo.bits_per_pixel == 24) {
        uint8_t* fb_ptr = (uint8_t*)framebuffer;
        index *= 3;
        fb_ptr[index] = color & 0xFF; //red
        fb_ptr[index + 1] = (color >> 8) & 0xFF; //green
        fb_ptr[index + 2] = (color >> 16) & 0xFF; //blue
    } else if (vinfo.bits_per_pixel == 32) {
        ((uint32_t*)framebuffer)[index] = color;
    }
}


void erase_pixel(int x, int y, uint32_t color) {
    if (x >= vinfo.xres || y >= vinfo.yres) {
        return;
    }
    int index = (y * vinfo.xres_virtual) + x;
    if (vinfo.bits_per_pixel == 16) {
        ((uint16_t*)framebuffer)[index] = color;
    } else if (vinfo.bits_per_pixel == 24) {
        uint8_t* fb_ptr = color;
        index *= 3;
        fb_ptr[index] = color & 0xFF;
        fb_ptr[index + 1] = color >> 8 & 0xFF;
        fb_ptr[index + 2] = color >> 16 & 0xFF;
    } else if (vinfo.bits_per_pixel == 32) {
        ((uint32_t*)framebuffer)[index] = color;
    }
}

void draw_mouse(int x, int y) {
    if (x >= vinfo.xres || y >= vinfo.yres) {
        return;
    }
    erase_cache_x = x;
    erase_cache_y = y;
    for (int dy = 0; dy < MOUSE_SIZE; dy++) {
        for (int dx = 0; dx < MOUSE_SIZE; dx++) {
            background_cache[dy][dx] = get_pixel(x + dx, y + dy);
        }
    }
    for (int dy = 0; dy < MOUSE_SIZE; dy++) {
        for (int dx = 0; dx < MOUSE_SIZE; dx++) {
            if (cursor[dy][dx] != 0x000000) {
                draw_pixel(x + dx, y + dy, cursor[dy][dx]);
            }
        }
    }
}

void erase_mouse(int x, int y) {
    if (erase_cache_x >= vinfo.xres || erase_cache_y >= vinfo.yres) {
        return;
    }
    for (int dy = 0; dy < MOUSE_SIZE; dy++) {
        for (int dx = 0; dx < MOUSE_SIZE; dx++) {
            draw_pixel(x + dx, y + dy, background_cache[dy][dx]);
        }
    }
}

void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int pos_y = 0; pos_y < h; pos_y++) {
        for (int pos_x = 0; pos_x < w; pos_x++) {
            draw_pixel(x + pos_x, y + pos_y, color);
        }
    }
}

void erase_rect(int x, int y, int w, int h) {
    for (int pos_y = 0; pos_y < h; pos_y++) {
        for (int pos_x = 0; pos_x < w; pos_x++) {
            draw_pixel(x + pos_x, y + pos_y, BLACK32);
        }
    }

}

void draw_highlight(int x, int y, int w, int h, uint32_t color) {
    for (int pos_x = x; pos_x < x + w; pos_x++) {
        draw_pixel(pos_x, y, color);
        draw_pixel(pos_x, y + h, color);
    }
    for (int pos_y = y; pos_y < y + h; pos_y++) {
        draw_pixel(x, pos_y, color);
        draw_pixel(x + w- 1, pos_y, color);
    }
}

void draw_top_bar(int w, int h, uint32_t color) {
    if (!disable_top_bar) {
        unsigned char shape[8] = {0x42, 0x24, 0x18, 0x18, 0x18, 0x24, 0x42, 0x00};
        draw_rect(0, 0, w, h, color);
        border_1_thickness = h;
        if (!disable_center_exit){
            draw_char(screen_res_x / 2, 0, 1, 1, shape, 0x000000);
            draw_char(screen_res_x / 2, 1, 1, 1, shape, 0x000000);
        } else {
            draw_char(screen_res_x - 8, 0, 1, 1, shape, 0x000000);
            draw_char(screen_res_x - 8, 1, 1, 1, shape, 0x000000);
        }
    }
}

void draw_bottom_bar(int w, int h, uint32_t color) {
    if (!disable_bottom_bar) {
        draw_rect(0, screen_res_y - h, w, h, color);
        border_3_thickness = h;
    }
}

void draw_left_bar(int w, int h, uint32_t color) {
    if (!disable_left_bar) {
        unsigned char shapes[64][8] =
        {{0x42, 0x24, 0x18, 0x18, 0x18, 0x24, 0x42, 0x00},
        {0x42, 0x24, 0x18, 0x18, 0x18, 0x24, 0x42, 0x00},
        {0x42, 0x24, 0x18, 0x18, 0x18, 0x24, 0x42, 0x00},
        {0x42, 0x24, 0x18, 0x18, 0x18, 0x24, 0x42, 0x00},
        {0x42, 0x24, 0x18, 0x18, 0x18, 0x24, 0x42, 0x00},
        };
        draw_rect(0, 0, w, h, color);
        border_4_thickness = w;
        /*
        not being worked on yet
        */
        if (!disable_draw_icon) {
            draw_char(1912, 0, 1, 1, shapes, 0x000000);
            draw_char(1912, 1, 1, 1, shapes, 0x000000);
        }
    }
}

void draw_right_bar(int w, int h, uint32_t color) {
    if (!disable_right_bar) {
        draw_rect(screen_res_x - w, 0, w, h, color);
        border_2_thickness = w;
    }
}

void draw_window(int w_1, int w_2, int w_3, int w_4, int h_1, int h_2, int h_3, int h_4, uint32_t color_1, uint32_t color_2, uint32_t color_3, uint32_t color_4, int win_preset, int anim_preset) {
    switch (win_preset) { //draw right side first or else it can draw on top of the X
        case 1:
            disable_center_exit = 0;
            draw_right_bar(w_3, h_3, color_3);
            draw_right_bar(w_3, h_3 / 4, 0x000000);
            draw_left_bar(w_4, h_4, color_4);
            draw_left_bar(w_4, h_4 / 4, 0x000000);

            draw_top_bar(w_1, h_1, 0x000000);
            draw_top_bar(w_1 / 2 + w_1 / 6, h_1, color_1);
            draw_top_bar(w_1 / 3, h_1, 0x000000);
            draw_bottom_bar(w_1, h_1, color_2);
            draw_bottom_bar(w_1 / 2 + w_1 / 6, h_1, 0x000000);
            draw_bottom_bar(w_1 / 3, h_1, color_2);
            break;
        case 2:
            draw_top_bar(w_1 + 4, h_1, color_1);
            draw_bottom_bar(w_2 + 4, h_2, color_2);
            break;
        case 3:
            draw_right_bar(w_3, h_3, color_3);
            draw_left_bar(w_4, h_4, color_4);
            break;
        case 4:
            draw_right_bar(w_3, h_3, color_3);
            draw_top_bar(w_1, h_1, color_1);
            draw_left_bar(w_4, h_4, color_4);
            break;
        case 5:
            draw_bottom_bar(w_2, h_2, color_2);
            draw_right_bar(w_3, h_3, color_3);
            draw_left_bar(w_4, h_4, color_4);
            break;
        case 6:
            draw_right_bar(w_3 * 4, h_3 * 4, color_3);
            draw_top_bar(w_1 * 4, h_1 * 4, color_1);
            draw_bottom_bar(w_2 * 4, h_2 * 4, color_2);
            draw_left_bar(w_4 * 4, h_4 * 4, color_4);
            break;
        case 7: //rest is unfinished
            draw_top_bar(w_1, h_1, color_1);
            draw_bottom_bar(w_2, h_2, color_2);
            draw_right_bar(w_3, h_3, color_3);
            draw_left_bar(w_4, h_4, color_4);
            break;
        case 8:
            draw_top_bar(w_1, h_1, color_1);
            draw_bottom_bar(w_2, h_2, color_2);
            draw_right_bar(w_3, h_3, color_3);
            draw_left_bar(w_4, h_4, color_4);
            break;
        default:
            draw_top_bar(w_1, h_1, color_1);
            draw_bottom_bar(w_2, h_2, color_2);
            draw_right_bar(w_3, h_3, color_3);
            draw_left_bar(w_4, h_4, color_4);
            break;
    }
}

void init_apps() {
    FILE *file = fopen("config/apps.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *name = strtok(line, ",");
       //printf("%s\n", name);
        printout(cell_width / strlen(name), cell_height, name, 0xFFFFFF);
    }
}

void check_apps(int x, int y) {

    FILE *file = fopen("config/apps.txt", "r");
    int app_x, app_y;
    if (!file) {
        perror("file not found, make apps.txt in config folder\n");
    }
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *name = strtok(line, ",");
        char *executable = strtok(NULL, ",");
        char *xy_pos = strtok(NULL,"\n");
        if (name && executable && xy_pos) {
            sscanf(xy_pos, "%d:%d", &app_x, &app_y);
            if (app_x == x && app_y == y) {
                if (cont == 1 && app_x == cont_x && app_y == cont_y) {
                    cont = 0;
                    cont_y = NULL;
                    cont_x = NULL;
                    char path[256];
                    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
                    path[len] = '\0';
                    char *dir = dirname(path);
                    strcat(dir, "/");
                    strcat(dir, executable);
                    clear_screen();
                    close_mouse();
                    close_keys();
                    close_framebuffer();
                    system(dir);
                    init_framebuffer();
                    init_keys();
                    init_mouse();
                    break;
                } else {
                    if (cont < 2) {
                        cont++;
                    }
                    cont_x = app_x;
                    cont_y = app_y;
                    break;
                }
            }
        }
    }
    fclose(file);
}



