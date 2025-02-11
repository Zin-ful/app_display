#include <stdio.h>
#include <string.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "keyboard.h"
#include "text_loader.h"
#include "windower.h"
#define INP_PATH "/dev/input/event0"

struct keyboard_event {
    struct timeval time;
    unsigned short type; //eventype like EV_REL
    unsigned short code; //keycode
    unsigned int value; //0 - release : 1 - press : 2 - hold
};

int kb_fd = -1;

int init_keys() {
    kb_fd = open(INP_PATH, O_RDONLY);
    if (kb_fd == -1) {
        perror("keyboard event failed to open\n");
        return -1;
    }
    return 0;
}

void read_keys() {
    struct keyboard_event inpt_evnt;
    while (read(kb_fd, &inpt_evnt, sizeof(inpt_evnt)) > 0) {
        if (inpt_evnt.type == EV_KEY && inpt_evnt.value == 1) {
            print(1 / text_cell_width, 1 / text_cell_height, 8 / text_cell_width, 8 / text_cell_height, "A",0xFFFFFF);
        }
    }
}

void close_keys() {
    if (kb_fd != -1) close(kb_fd);
}
