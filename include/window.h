#ifndef WINDOW
#define WINDOW

#include <gb.h>
#define SCREEN_WIDTH  160
#define SCREEN_HEIGHT 144



void window_init();



void window_drawFrame(uint16_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH]);
void window_close();










#endif