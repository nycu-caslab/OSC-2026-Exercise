extern char uart_getc(void);
extern void uart_putc(char c);
extern void uart_puts(const char* s);
extern void uart_hex(unsigned long h);
extern void video_init();
extern void video_bmp_display(unsigned int* bmp_image, int width, int height);

#define TIME_FREQ 10000000
void sleep(int msec) {
    // TODO: Implement this function
}

void display_video() {
#include "bird.h"
    while (1) {
        for (int f = 0; f < FRAME_COUNT; f++) {
            unsigned int* frame = (frames + (f * FRAME_WIDTH * FRAME_HEIGHT));
            video_bmp_display(frame, FRAME_WIDTH, FRAME_HEIGHT);
            sleep(100);
        }
    }
}

void start_kernel() {
    uart_puts("\nStarting kernel ...\n");
    // TODO: Initialize the QEMU frame buffer device
    display_video();
}
