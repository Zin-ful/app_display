#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#define MOUSE_PATH "/dev/input/mice"

int mouse_fd = -1;

int init_mouse() {
    mouse_fd = open(MOUSE_PATH, O_RDONLY);
    if (mouse_fd == -1) {
        perror("failed to open mouse event\n");
        return -1;
    }
    return 0;
}

void read_mouse() {
    unsigned char data[3];
    while (read(mouse_fd, data, sizeof(data)) > 0) {
        int left = data[0] & 0x1;
        int right = data[0] & 0x2;
        int middle = data[0] & 0x4;
        int dx = data[1];
        int dy = data[2];

        printf("Mouse: Left[%d] Right[%d] Middle[%d] Pos X[%d] Pos Y[%d]\n", left, right, middle, dx, dy);
    }
}

void close_mouse() {
    if (mouse_fd != -1) close(mouse_fd);
}
