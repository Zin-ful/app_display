/*
avaliable functions:
//windows
init_framebuffer()
clear_screen()
close_framebuffer()
draw_rect(x, y, w, h, color)
draw_pixel(x, y, color)
draw_highlight(x, y, w, h, color)
//keyboard
init_keys()
read_keys()
//mouse
init_mouse()
read_mouse()
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "windower.h"
#include "keyboard.h"
#include "mouse.h"

pthread_t keys_thread;
pthread_t mouse_thread;
pthread_t frames_thread;
int running = 1;
void* keys_task(void* arg) {
    while (running) {
        read_keys();
        usleep(20000);
    }
}

void* mouse_task(void *arg) {
    while (running) {
        read_mouse();
        usleep(10000); //usleep 10k in mouse.c
    }
}

void* frames_task(void *arg) {
    while (running) {
        //draw_pixel(mouse_pos_x, mouse_pos_y, 0xFFFFFF);
        //erase_pixel(mouse_pos_x, mouse_pos_y);
        usleep(20000); //stick to 20k for now, define defaults later. prolly check cpu usage every 2 seconds and increase buffer if larger than 90
    }
}

int main() {
    if (init_framebuffer() != 0) {
        fprintf(stderr, "error init frames (main)\n");
        return 1;
    }
    if (init_mouse() != 0) {
        fprintf(stderr, "error init mouse (main)\n");
        return 1;
        close_framebuffer();
    }
    if (init_keys() != 0) {
        fprintf(stderr, "error init keys(main)\n");
        close_keys();
        close_framebuffer();
        return 1;
    }
    clear_screen();
    pthread_create(&keys_thread, NULL, keys_task, NULL);
    pthread_create(&mouse_thread, NULL, mouse_task, NULL);
    pthread_create(&frames_thread, NULL, frames_task, NULL);

    while (running) {
        usleep(20000); //2nd buffer for cpu
    }


    pthread_join(keys_thread, NULL);
    pthread_join(mouse_thread, NULL);
    pthread_join(frames_thread, NULL);

    close_mouse();
    close_keys();
    close_framebuffer();
}
