/* window.h     gb-emu
    handles the SDL2 lib to display pixels on the screen



*/

#ifndef WINDOW
#define WINDOW

#include <gb.h>
#define SCREEN_WIDTH  160
#define SCREEN_HEIGHT 144


/* void window_init(void)
    initilices the SDL2 windows. exeperimental right now
*/
void window_init();
// frees all memory
void window_close();

// draw current frambufer onto screen
void window_drawFrame(uint16_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH]);

// gets user input
void window_getIO(uint8_t * io);









#endif