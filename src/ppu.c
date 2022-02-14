#include <gb.h>


#define SCREEN_WIDTH  160
#define SCREEN_HEIGHT 144

#define INC_LY mem.LY++;

/* 
__     __         _       _     _
\ \   / /_ _ _ __(_) __ _| |__ | | ___  ___
 \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|
  \ V / (_| | |  | | (_| | |_) | |  __/\__ \
   \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/
 */

// global variables
ppu_t ppu;
uint8_t framebuffer[SCREEN_WIDTH][SCREEN_HEIGHT] ;
extern mem_t mem;
extern cpu_t cpu;


// faek pallete for the moement
uint16_t mono_pallete[4] = {0x7FFF, 0x7777, 0x2222, 0x000};




void ppu_init(){

    memset(framebuffer, 0, SCREEN_WIDTH*SCREEN_HEIGHT);
    ppu.currentX = 0;
    ppu.newFrame = 0;

    ppu.clock = 0;
    ppu.state = OAM_SEARCH;
    mem.LY = 0;

}


void ppu_tick(){
    uint32_t currentCycle = ppu.clock++ % 115; //catch ip to the cpu cycle. modulo 115. every line is 115 "ticks" wide (OAM search -> pioxel transfer -> hblank)
    //ppu.clock++;
    switch (ppu.state)
    {
        case OAM_SEARCH:

            if ( currentCycle == 20)
            {
                // get sprite data for line
                ppu.spriteSize = mem.LCDC & ( 1<<2 ) ? 16 : 8;
                for (uint8_t i = 0; i < 40; i++)
                {
                    
                    ppu.oam_sprites[i].y      = mem_read(0xFE00 + i*4   );
                    ppu.oam_sprites[i].x      = mem_read(0xFE00 + i*4 +1);
                    ppu.oam_sprites[i].tilenr = mem_read(0xFE00 + i*4 +2);
                    uint8_t atr                = mem_read(0xFE00 + i*4 +3);
                    ppu.oam_sprites[i].prio    = atr & (1<<7);
                    ppu.oam_sprites[i].y_flip  = atr & (1<<6);
                    ppu.oam_sprites[i].x_flip  = atr & (1<<5);
                    ppu.oam_sprites[i].palette = atr & (1<<4);
                }

                ppu.objEnable = !!(mem.LCDC & (1<<1)); // get object enable flag
                ppu.state = PIXEL_TRANSFER;
                
                mem.STAT =  (mem.STAT & 0xFC) + ppu.state;
                //mem_write(STAT,  ( mem_read(STAT) & (0xFC | ppu.state)) );//TODO CHECK
            }
            break;


        
        case PIXEL_TRANSFER:
           // draw_Line(ppu);

            if (currentCycle == 64)
            {
                ppu_drawLine();;
                ppu.state = HBLANK;
                INC_LY;
                
                
                mem.STAT =  (mem.STAT & 0xFC) + ppu.state;
                //mem_write(STAT,  ( mem_read(STAT) & (0xFC | ppu.state)) ); //TODO CHECK
                //if (VMEM(STAT) & ( 1 << 3)) TRIGGER_IRQ(IRQ_LCDC);
                if (mem.STAT & ( 1 << 3) ) TRIGGER_IRQ(IRQ_LCDC);


            }
            break;



        case HBLANK:    
            
            if ( currentCycle == 114 && mem.LY < 144 ) 
            { 
                

                ppu.state = OAM_SEARCH;
                
                //VMEM(LCDC) = (VMEM(LCDC)) & 0xFC | ppu.state;
                mem.STAT =  (mem.STAT & 0xFC) + ppu.state;
                //mem_write(STAT,  ( mem_read(STAT) & (0xFC | ppu.state)) );
                if (mem.STAT & ( 1 << 5)) TRIGGER_IRQ(IRQ_LCDC);

            }
            else if ( currentCycle == 114 && mem.LY  >= 144 )
            {
                INC_LY;

                ppu.state = VBLANK;

                //VMEM(LCDC) = (VMEM(LCDC)) & 0xFC | ppu.state; 
                mem.STAT =  (mem.STAT & 0xFC) + ppu.state;
                //mem_write(STAT,  ( mem_read(STAT) & (0xFC | ppu.state)) ); //TODO CHECK
                if (mem.STAT & ( 1 << 4)) TRIGGER_IRQ(IRQ_LCDC);
                TRIGGER_IRQ(IRQ_VBLANK);
            }
            break;



        case VBLANK:

            if (currentCycle == 114) 
            {                
                INC_LY;
            }

            if (mem.LY  >= 154)
            {
                
                mem.LY = 0 ;
                ppu.state = OAM_SEARCH;
                ppu.newFrame = 1;

                //VMEM(LCDC) = VMEM(LCDC) & 0xFC | ppu.state;
                mem.STAT =  (mem.STAT & 0xFC) + ppu.state;
                //mem_write(STAT,  ( mem_read(STAT) & (0xFC | ppu.state)) ); //TODO CHECK
                if (mem.STAT & ( 1 << 5)) TRIGGER_IRQ(IRQ_LCDC);

            }
                
            break;

            
    }


    // Check if LY == LYC and set bit - if enabled, trigger interrupt    
    if ( (mem.LY  == mem.LYC) )
    {
        
        mem.STAT |= ( 1 << 2); //VMEM(STAT) |= ( 1 << 2);
        
        if ( mem.STAT & ( 1 << 6 ) )
        {            
            TRIGGER_IRQ(IRQ_LCDC);
        } 
    } 

    else
    {
        mem.STAT &= ~( 1 << 2 ) ; //VMEM(STAT) &= ~( 1 << 2 );
        
    }


}


// draw line

void ppu_drawLine(){


    //draw Background    
    // Tile select
    // start at 8000, signed
    uint16_t tileBaseAdr;
    uint16_t mapBaseAdr;
    uint16_t currentTileAdr;
    uint16_t currentMapAdr;

    uint8_t singedmap = 0;
    if ( mem.LCDC & ( 1 << 4 ) )
    {
        tileBaseAdr = 0x8000;
    }
    // start at 8800, unsigned
    else
    {
        tileBaseAdr = 0x9000;
        singedmap = 1;
    }

    // BG MAp select start at  9800
    mapBaseAdr = mem.LCDC & ( 1 << 3) ? 0x9C00 : 0x9800;

    uint8_t tileByte1, tileByte2;


    
    for (uint8_t j = 0; j < 160; j++)
    {
        // read tile num from bg map
        currentMapAdr = (((mem.SCY + mem.LY) / 8) % 32) * 32 + (((mem.SCX + j) / 8) % 32);
        if (singedmap == 1)
            currentTileAdr = ((int8_t) mem_read(currentMapAdr + mapBaseAdr)* 4) + tileBaseAdr;
       
        else
            currentTileAdr =  (mem_read(currentMapAdr + mapBaseAdr) << 4) + tileBaseAdr;

        
        // read tile bytes (2 tiles per line) for current LY line
        tileByte1 = mem_read( currentTileAdr + ( (mem.LY % 8)*2)    );
        tileByte2 = mem_read( currentTileAdr + ( (mem.LY % 8)*2) +1 );
        //set pixel
        // if mono gb mode
        uint8_t x = (( j + mem.SCX ) % 8); 
        ppu.framebuffer[mem.LY][j] = mono_pallete[ ( ( tileByte1 >> (6-x) ) & 0x2 ) | ( (tileByte2 >> (7-x)) & 0x01) ];  
    }


/*
    //draw sprite
    
    uint8_t drawnSprites = 0;
    uint8_t currentIndex = VMEM(LY);

    for (uint8_t j = 0; j < 40; j++){

        // check if sprite is in y line
        if (ppu.oam_sprites[j].y-8 > currentIndex && ppu.oam_sprites[j].y-16 <= currentIndex && ppu.objEnable)
        {
            //if outside x screen
            if (ppu.oam_sprites[j].x == 0 || ppu.oam_sprites[j].x >= 168) {
                drawnSprites++;
                continue;
            }


            currTile = 0x8000 |  ppu.oam_sprites[j].tilenr << 4;

            if ( ppu.oam_sprites[j].y_flip){
                uint8_t idx = (((ppu.oam_sprites[j].y-9)) - (VMEM(LY))  );
                t1 = VMEM( currTile + idx*2   ) ;
                t2 = VMEM( currTile + idx*2 +1 ) ;
            }
            else{
                t1 = VMEM( currTile + ((VMEM(LY) - (ppu.oam_sprites[j].y-16)) *2)    ) ;
                t2 = VMEM( currTile + ((VMEM(LY) - (ppu.oam_sprites[j].y-16)) *2) +1 ) ;
            }
            uint8_t pixelCol = 0;
            if (ppu.oam_sprites[j].x_flip ){
                for (uint8_t i = 0; i < 8; i++){
                    pixelCol = (!!(t1 & (0x01 << i))) + (!!(t2 & (0x01 << i)))*2;

                    if (pixelCol != 0){
                        ppu.framebuffer[VMEM(LY)][ i + ppu.oam_sprites[j].x -8] = pixelCol;
                    }
                }
            }
            else {
                for (uint8_t i = 0; i < 8; i++){
                    pixelCol = (!!(t1 & (0x80 >> i))) + (!!(t2 & (0x80 >> i)))*2;
                    
                    if (pixelCol != 0){
                        ppu.framebuffer[VMEM(LY)][ i + ppu.oam_sprites[j].x -8] = pixelCol;
                    }
                    
                }
            }

            drawnSprites++;


        }
    }
*/

}


