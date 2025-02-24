#include <stdio.h>
#include <string.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include "keyboard.h"
#include "text_loader.h"
#include "windower.h"
#define INP_PATH "/dev/input/event0"
int input_ready;
int allow_typing = 1;
int x = 0;
int y = 0;
char user_inp[610];
int recent_key_pressed;
keyboard_event_t inpt_event;

int kb_fd = -1;

int init_keys() {
    kb_fd = open(INP_PATH, O_RDONLY);
    if (kb_fd == -1) {
        perror("keyboard event failed to open\n");
        return -1;
    }
    return 0;
}

char* pass_inp() {
    return user_inp;
}

void read_keys() {
    struct keyboard_event inpt_evnt;
    int cursor_x = x / text_cell_width;
    int cursor_y = y / text_cell_height;
    while (read(kb_fd, &inpt_evnt, sizeof(inpt_evnt)) > 0) {
        if (inpt_evnt.type == EV_KEY && inpt_evnt.value == 1 && allow_typing == 1) {
            recent_key_pressed = inpt_evnt.code;
            print(text_cell_width, text_cell_height, inpt_evnt.code , 0xFFFFFF);
        }
    }
}

void read_inp() {
    while(1) {
        fgets(user_inp, sizeof(user_inp), stdin);
        if (user_inp[strlen(user_inp) - 1] == '\n') {
            user_inp[strlen(user_inp) - 1] = '\0';
            input_ready = 1;
            pass_inp();
            memset(&user_inp, 0, sizeof(user_inp));
            input_ready = 0;
        }
    }
}

void close_keys() {
    if (kb_fd != -1) close(kb_fd);
}
