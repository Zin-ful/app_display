#include <stdio.h>
#include "../../keyboard.h"
#include "windower.h"
#include "text_loader.h"
#include "pthread.h"
pthread_t get_keys_thread;

void get_keys_task(void *arg) {
     read_inp();
     usleep(10000);
}

int main() {
    allow_typing = 1;
    pthread_create(&get_keys_thread, NULL, get_keys_task, NULL);
    while (1) {
        if (input_ready = 1) {
            char* user_text = pass_inp();
            printf("%s\n", user_text);
        }
    }
    pthread_join(get_keys_thread, NULL);
}
