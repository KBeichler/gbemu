#ifndef MEM
#define MEM
#include <gb.h>
#include <stdlib.h>
#include <string.h>



typedef struct mem_t
{
    uint8_t *loadedRom;
    uint32_t loadedRomLength;

    uint8_t activeRomBank;
    uint8_t activeRamBank;

	uint8_t cartType;
	uint8_t cartRomBankCount;
	uint8_t cartRamBankCount;


    uint8_t bootRom[0x100];


    //uint8_t *fixedRom;
    //uint8_t *switchedRom;
    uint8_t *eRam;

    uint8_t vRam[0x2000];    
    uint8_t fixedRam[0x1000];
    uint8_t switchedRam[0x8][0x1000];

	uint8_t oam[0xA0];
    union
    {
        uint8_t io[0x80];
        struct 
        {
            uint8_t P1;
            uint8_t SB;
            uint8_t SC;
            uint8_t DIV;
            uint8_t TIMA;
            uint8_t TMA;
            uint8_t TAC;
            uint8_t _offset [0x08];
            uint8_t IF;
            uint8_t _offset2 [ 0x30];
            uint8_t LCDC;
            uint8_t STAT;
            uint8_t SCY;
            uint8_t SCX;
            uint8_t LY;
            uint8_t LYC;
            uint8_t DMA;
            uint8_t BGP;
            uint8_t OBP0;
            uint8_t OBP1;
            uint8_t WY;
            uint8_t WX;
        };
        
    };

    uint8_t hRam[0x80];
    uint8_t IE;

    uint8_t DMA_active;


}mem_t;






void mem_init();
uint8_t mem_read(uint16_t adr);
uint8_t mem_write(uint16_t adr, uint8_t val);
uint8_t mem_loadRom(char* path);
void mem_close();


void mem_doDMA();



void mem_setRomBankMBC1(uint16_t adr, uint8_t val);





#endif