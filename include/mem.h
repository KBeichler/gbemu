#ifndef MEM
#define MEM
#include <gb.h>
#include <stdlib.h>
#include <string.h>



typedef struct mem_t
{
    uint8_t *loadedRom;

    uint8_t activeRomBank;
    uint8_t activeRAMBank;

	uint8_t cartType;
	uint8_t cartRomSize;
	uint8_t cartRamSize;


    uint8_t bootRom[0x100];


    uint8_t *fixedRom;
    uint8_t *switchedRom;
    uint8_t *eRam;

    uint8_t vRam[0x2000];    
    uint8_t fixedRam[0x1000];
    uint8_t switchedRam[0x8][0x1000];

	uint8_t oam[0xA0];
    uint8_t io[0x80];

    uint8_t hRam[0x80];
    uint8_t irq;


}mem_t;










void mem_init();
uint8_t mem_read(uint16_t adr);
uint8_t mem_write(uint16_t adr, uint8_t val);
uint8_t mem_loadRom(char* path);
void mem_close();






#endif