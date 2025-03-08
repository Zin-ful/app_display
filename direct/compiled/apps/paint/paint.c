/*

disable_click_host
disable_app_check_host
disable_typing
disable_exit_button
*/


#include <stdio.h>
#include "../../windower.h"
#include "../../mouse.h"
#include "../../keyboard.h"
#include <linux/input.h>
#include <pthread.h>

pthread_t mouse_thread;
pthread_t paint_thread;
pthread_t keys_thread;

uint32_t color_arr[50] = {0xFFFFFF, 0xFF0D0D, 0x0D1DFF, 0x0DFF1D, 0xA60DFF, 0x0DBEFF, 0x86FF0D, 0xFFEE0D, 0xFFB60D, 0xD096EE, 0xEEE269};
int color_total = 10;
void* mouse_task(void *arg) {
    while (1) {
        read_mouse();
    }
}

void* keys_task(void *arg) {
    while (1) {
        read_keys();
    }
}

void* paint_task(void *arg) {
    int current_color = 0;
    while (1) {
        if (left_click) {
            draw_rect(mouse_pos_x - 2, mouse_pos_y - 2, 4, 4, color_arr[current_color]);
        } else if (right_click) {
            erase_rect(mouse_pos_x - 8, mouse_pos_y - 8, 20, 20);
        } else if (middle_click) {
            current_color++;
            if (current_color > color_total) {
                current_color = 0;
            }
            usleep(400000);
        } else if (recent_key_pressed == KEY_SPACE) {
            clear_screen(); //no worky
        }
    }
}



int main(void) {
    disable_click_host = 1;
    disable_check_app_host = 1;
    disable_typing = 1;
    left_click = 0;
    right_click = 0;
    middle_click = 0;

    init_framebuffer();
    init_mouse();
    init_keys();
    //clear_screen();
    printf("%s", get_char(KEY_A));
    pthread_create(&mouse_thread, NULL, mouse_task, NULL);
    pthread_create(&paint_thread, NULL, paint_task, NULL);
    pthread_create(&keys_thread, NULL, keys_task, NULL);
    while (1) {
        usleep(10000);
    }
    pthread_join(&mouse_thread, NULL);
    pthread_join(&paint_thread, NULL);
}
