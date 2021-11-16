#include "gb_t.h"


#define VMEM(mr) ppu->mem->data[mr]

#define TRIGGER_IRQ(I)  VMEM(I_FLAG) |= (1 << I); 

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


    uint8_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH] ;
    uint32_t cycle;
    ppu_state_e state;
    mem_t *mem;
    uint8_t currx;
    uint8_t newFrame;
    oam_sprite_t oam_sprites[40];
    uint8_t spriteSize;
    uint8_t objEnable;
}ppu_t;



uint8_t init_ppu(ppu_t *, cpu_t *);


void tick_ppu(ppu_t *);

void draw_Line(ppu_t *);