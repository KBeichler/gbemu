/*  ppu.h   gb-emu  PIXEL-PROCESSING-UNIT
*   used in the gb-emu project. ppu.h and ppu.c handel all the Graphics
*   
*   
*    2022.02.13: currently all pixels are written into a framebuffer inside the ppu struct
+                   -> classic gameboy, 2 bit color depth is used. Palette is taken from internal memry 
*                   -> sprites and background are not implemeted yet
*                   -> ppu.h uses a simple state machine to cycle to the 4 states (OAM SEARCH, PIXEL TRANSFER, HBLANK, VBLANK)
*
*
*   
*
*/
#ifndef PPU
#define PPU
#include <gb.h>


/*
     _       _              _       __ _       _ _   _                 
  __| | __ _| |_ __ _    __| | ___ / _(_)_ __ (_) |_(_) ___  _ __  ___ 
 / _` |/ _` | __/ _` |  / _` |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
| (_| | (_| | || (_| | | (_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 \__,_|\__,_|\__\__,_|  \__,_|\___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/
                                                                     
*/

// Statemachine states
typedef enum ppu_state_e{
    HBLANK = 0,
    VBLANK,
    OAM_SEARCH,
    PIXEL_TRANSFER
}ppu_state_e;

// sprite data struct
typedef struct oam_sprite_t{
    uint8_t y;          // current y position on screen
    uint8_t x;          // current x position on screen
    uint16_t tilenr;    // tile nr -> references to tile ram
    uint8_t prio;       // priority for overlap control
    uint8_t x_flip;     
    uint8_t y_flip;
    uint8_t palette;    // color palette user for sprite
}oam_sprite_t;

// ppu struct
typedef struct ppu_t{
    
    ppu_state_e state;

    oam_sprite_t oam_sprites[40];   // sprite buffer
    uint8_t spriteSize;             // sprite size helper variable
    uint16_t framebuffer[144][160]; 


    // info flags
    uint8_t objEnable;
    uint8_t currentX;
    uint8_t newFrame;
    uint32_t clock;
}ppu_t;


/* void ppu_init(void)  
    initilizces ppu and variables
*/
void ppu_init(void);

/* void ppu_tick(void)
    main ppu function. catches up to the cpu cycle. handles the statemachine, calls the oam search and the drawing of pixels
*/
void ppu_tick();

/*  void ppu_drawLine(void)
    draws current Line (traken from mem registers) to the framebuffer
    sprites currently no implemented
*/
void ppu_drawLine(void);




























































#endif