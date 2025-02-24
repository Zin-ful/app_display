#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <sys/time.h>

extern int allow_typing;
extern int input_ready;
extern int recent_key_pressed;

typedef struct keyboard_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
} keyboard_event_t;

extern keyboard_event_t inpt_event;

void read_keys();
void read_inp();
char* pass_inp();
int init_keys();
void close_keys();
#endif
