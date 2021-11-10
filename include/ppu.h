#ifndef PPU
#define PPU
#include <gb.h>


#define TRIGGER_IRQ(I)  {uint8_t r = mem_read(I_FLAG); mem_write(I_FLAG, r | (1 << I));}

typedef enum ppu_state_e{
    HBLANK = 0,
    VBLANK,
    OAM_SEARCH,
    PIXEL_TRANSFER
}ppu_state_e;


typedef struct oam_sprite_t{
    uint8_t y;
    uint8_t x;
    uint16_t tilenr;
    uint8_t prio;
    uint8_t x_flip;
    uint8_t y_flip;
    uint8_t palette;
}oam_sprite_t;

typedef struct ppu_t{
    
    
    ppu_state_e state;

    oam_sprite_t oam_sprites[40];
    uint8_t spriteSize;


    // info flags
    uint8_t objEnable;
    uint8_t currentX;
    uint8_t newFrame;
    uint32_t clock;
}ppu_t;



void ppu_init();

void ppu_tick();




























































#endif