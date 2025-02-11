
/*

main.c IS ONLY INCLUDED IN THE windower.c FILE BECAUSE OF THE check_app FUNCTION (linked to screen_event)
NO OTHER FILES INCLUDE MAIN
WILL TRY AND FIX LATER
I CANT COPY AND PASTE

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>
#include "windower.h"
#include "keyboard.h"
#include "mouse.h"
#include "text_loader.h"

int cont = 0;
int cont_x, cont_y; //verify app selection in check_apps
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

void check_apps(int x, int y) {
    FILE *file = fopen("config/apps.txt", "r");
    int app_x, app_y;
    if (!file) {
        perror("there is no apps.txt inside of config.\neither fix or reinstall.\n\nformat:\nname,path, xy\nexample:\nnotes,/usr/bin/notesapp,00\napps arent stored in bin normally, their stored here in the apps folder.\n");
        return -1;
    }
    char line[1024];

    while (fgets(line, sizeof(line), file) != NULL) {
        //printf("%s\n", line);
        char *name = strtok(line, ",");
        char *executable = strtok(NULL, ",");
        char *xy_pos = strtok(NULL, "\n");
        if (name && executable && xy_pos) {
            sscanf(xy_pos, "%d:%d", &app_x, &app_y);
            if (app_x == x && app_y == y) {
              //printf("\APPFOUND\nname: %s\nlocation: %s\nposition: %d,%d\nmouse: %d,%d\n\n", name, executable, app_x, app_y, x, y);
                if (executable) {
                    if (cont == 1 && app_x == cont_x && app_y == cont_y) {
                        cont = 0;
                        cont_x = NULL;
                        cont_y = NULL;
                        char path[256];
                        ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
                        path[len] = '\0';
                        char *dir = dirname(path);
                        remove_space(dir);
                        remove_space(executable);
                        strcat(dir, "/");
                        strcat(dir, executable);
                        //printf("full exec path: %s\n", dir);
                        clear_screen();
                        close_mouse();
                        close_keys();
                        close_framebuffer();
                        system(dir);
                        init_framebuffer();
                        init_keys();
                        init_mouse();
                        clear_screen();
                        break;
                    } else {
                        if (cont < 2){
                            cont++;
                        }
                        cont_x = app_x;
                        cont_y = app_y;
                        break;
                    }
                }
            } else {
                //printf("app not found\napp xy: %d:%d\nmouse xy: %d:%d\n", app_x, app_y, x, y);
            }
        }
    }
    fclose(file);
}

void remove_space(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i]; //idk wtf any of this is doing but we are copying non string chars
        }
        i++;
    }
    str[j] = '\0';
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
