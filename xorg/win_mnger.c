#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600


int main() {

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "connection to x failed\n");
        exit(1);
    }
    int screen = DefaultScreen(display);

    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, WIDTH, HEIGHT, 1, BlackPixel(display, screen), WhitePixel(display, screen));

    XStoreName(display, window, "Simple Window");

    XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);

    XMapWindow(display, window);

    XEvent event;

    while (1) {

        XNextEvent(display, &event);

        if (event.type == KeyPress) {
            break;
        }
        if (event.type == ClientMessage) {
            break;
        }
        if (event.type == Expose) {

        }
    }
    XCloseDisplay(display);

    return 0;
}
