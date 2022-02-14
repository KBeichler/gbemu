/* mem.h    gb-emu
    mem.h holds all functions to communicate with the memory. including cartridge rom, internal and external ram an IO registers.
    ROM Bank switching also gets handled by the mem lib.
    all memory access (read/write) have to happen over memh functions


 */
#ifndef MEM
#define MEM
#include <gb.h>
#include <stdlib.h>
#include <string.h>

/*
     _       _              _       __ _       _ _   _                 
  __| | __ _| |_ __ _    __| | ___ / _(_)_ __ (_) |_(_) ___  _ __  ___ 
 / _` |/ _` | __/ _` |  / _` |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
| (_| | (_| | || (_| | | (_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 \__,_|\__,_|\__\__,_|  \__,_|\___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/
                                                                     
*/
// Main mem_t struct: structure hold all memory. roms get loaded into array. 
typedef struct mem_t        
{
    uint8_t *loadedRom;         // Pointer to loaded rom
    uint32_t loadedRomLength;

    uint8_t activeRomBank;      // id for roms and ram banks
    uint8_t activeRamBank;

	uint8_t cartType;           // Helper variables
	uint8_t cartRomBankCount;
	uint8_t cartRamBankCount;


    uint8_t bootRom[0x100];     // boot rom buffer


    uint8_t *eRam;              // external ram, not implemented

    uint8_t vRam[0x2000];       // internal video ram
    uint8_t fixedRam[0x1000];   // internal ram
    uint8_t switchedRam[0x8][0x1000];   // switchable ram banks

	uint8_t oam[0xA0];          // oam ram
    union                       // unions hold all IO registers for easier accessing
    {
        uint8_t io[0x80];
        struct 
        {
            uint8_t P1;
            uint8_t SB;
            uint8_t SC;
            uint8_t _offset3;
            uint8_t DIV;
            uint8_t TIMA;
            uint8_t TMA;
            uint8_t TAC;
            uint8_t _offset [0x07];
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

    uint8_t hRam[0x80];         // high ram
    uint8_t IE;                 // interrupt enable reg

    //helper varaibles
    uint8_t DMA_active;         


}mem_t;



/* 
 _____                 _   _
|  ___|   _ _ __   ___| |_(_) ___  _ __
| |_ | | | | '_ \ / __| __| |/ _ \| '_ \
|  _|| |_| | | | | (__| |_| | (_) | | | |
|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|

     _       __ _       _   _
  __| | ___ / _(_)_ __ | |_(_) ___  _ __  ___
 / _` |/ _ \ |_| | '_ \| __| |/ _ \| '_ \/ __|
| (_| |  __/  _| | | | | |_| | (_) | | | \__ \
 \__,_|\___|_| |_|_| |_|\__|_|\___/|_| |_|___/

 */

/* void mem_init(void)
    initialices mem module, sets all pointers and cleans memory
*/
void mem_init(void);
/* void mem_close(void)
    frees all memory
*/
void mem_close(void);

/* uint8_t mem_read(uint16_t adr)
    returns byte from adr. 16bit addressses, 0x0000 - 0xffff
*/
uint8_t mem_read(uint16_t adr);

/* uint8_t mem_write(uint16_t adr, uint8_t val)
    writes one byte to adr
*/
uint8_t mem_write(uint16_t adr, uint8_t val);

/* uint8_t mem_loadRom(char* path);
    loads rom from path. allocates memory and puts pointer into global mem stuct
*/
uint8_t mem_loadRom(char* path);


/* void mem_doDMA(vooid)
    executes the DMA data transfer. Takes values from internal memory. DMA ist requested after write to register 0xFF46 (set DMAactive helper variable)
    function has to be called independently, resets DMAactive variabble
*/
void mem_doDMA(void);

/* void mem_setRomBankMBC1(uint16_t adr, uint8_t val);
    set rombank helper function. gets called by mem_write. switches the rombank info variables on the memstruct accordingly
*/
void mem_setRomBankMBC1(uint16_t adr, uint8_t val);





#endif