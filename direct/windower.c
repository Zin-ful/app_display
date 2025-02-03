#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>


int fb_fd = -1;
void* framebuffer = NULL;
struct fb_var_screeninfo vinfo;
#define BLACK16 0x0000
#define COLOR24 0x00
#define BLACK32 0xFF000000
#define WHITE32 0xFFFFFF
#define LILAC32 0xFF9999
#define LIME32 0xFF6666
#define GRAY32 0xFF7453
#define IDK32 0XFF1234
/*
basic utility until after draw_pixel() line 97
*/
int init_framebuffer() {
    //open fb0
    fb_fd = open("/dev/fb0", O_RDWR); //opens fb as rw | change if framebuffer is in other location
    if (fb_fd == -1) {
        perror("failed to open framebuffer\n");
        return -1;
    }

    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("failed reading framebuffer info");
        close(fb_fd);
        return -1;
    }
    printf("Framebuffer information: \n");
    printf("Resolution: %dx%d\n", vinfo.xres, vinfo.yres);
    printf("Color depth: %d bpp\n", vinfo.bits_per_pixel);
    
    framebuffer = mmap(NULL, vinfo.yres_virtual * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8), PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
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
    int bitspp = vinfo.bits_per_pixel / 8;

    if (vinfo.bits_per_pixel == 16) {
        uint16_t* fb_ptr = (uint16_t*)framebuffer;
        for (int i = 0; i < screensize; i++) {
            fb_ptr[i] = BLACK16;
        }
    } else if (vinfo.bits_per_pixel == 24) {
        uint8_t* fb_ptr = (uint8_t*)framebuffer;
        for (int i = 0; i < screensize * 3; i += 3) {
            fb_ptr[i] = COLOR24;
            fb_ptr[i + 1] = COLOR24;
            fb_ptr[i + 2] = COLOR24;
        }
    } else if (vinfo.bits_per_pixel == 32) {
        uint32_t* fb_ptr = (uint32_t*)framebuffer;
        for (int i = 0; i < screensize; i++) {
            fb_ptr[i] = BLACK32;
        }
    }

}

void draw_pixel(int x, int y, uint32_t color) {
    if (x >= vinfo.xres || y >= vinfo.yres) {
        return;
    }
    int index = (y * vinfo.xres_virtual) + x;
    //copy pasted, remove for loops and adjust
    if (vinfo.bits_per_pixel == 16) {
        ((uint16_t*)framebuffer)[index] = (uint16_t)color;
    } else if (vinfo.bits_per_pixel == 24) {
        uint8_t* fb_ptr = (uint8_t*)framebuffer;
        index *= 3;
        fb_ptr[index] = color & 0xFF;
        fb_ptr[index + 1] = (color >> 8) & 0xFF;
        fb_ptr[index + 2] = (color >> 16) & 0xFF;
    } else if (vinfo.bits_per_pixel == 32) {
        ((uint32_t*)framebuffer)[index] = color;
    }
}

void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int pos_y = 0; pos_y < h; pos_y++) {
        for (int pos_x = 0; pos_x < w; pos_x++) {
            draw_pixel(x + pos_x, y + pos_y, color);
        }
    }
