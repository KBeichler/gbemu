#include <gb.h>
#include <mem.h>

// Boot ROM of a standard gameboy, used for startup
static uint8_t BOOT_ROM[0x100] = 
{
	0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
	0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
	0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
	0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
	0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
	0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
	0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
	0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
	0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
	0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
	0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
	0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
	0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
	0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
	0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
	0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50 
};




/*
__     __         _       _     _
\ \   / /_ _ _ __(_) __ _| |__ | | ___  ___
 \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|
  \ V / (_| | |  | | (_| | |_) | |  __/\__ \
   \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/
 */
extern cpu_t cpu;   // global cpu struct
mem_t mem;          //global mem struct

// setRombank function pointer. Different cartirdges need different handling, thats why i work via a function pointer
// gets set on loading the ROM file
void (*_setRomBank) (uint16_t, uint8_t);




void mem_init(void)
{
    // init mem with 0
    memset((void *) &mem, 0, sizeof(mem_t)); 

    // copy bootrom to struct
    memcpy(mem.bootRom, BOOT_ROM, sizeof(BOOT_ROM)); 

    // Powerup seuqence as in Pandocs
    mem_write( 0xFF05 ,0x00 ); 
    mem_write( 0xFF06 ,0x00 ); 
    mem_write( 0xFF07 ,0x00 ); 
    mem_write( 0xFF10 ,0x80 ); 
    mem_write( 0xFF11 ,0xBF ); 
    mem_write( 0xFF12 ,0xF3 ); 
    mem_write( 0xFF14 ,0xBF ); 
    mem_write( 0xFF16 ,0x3F ); 
    mem_write( 0xFF17 ,0x00 ); 
    mem_write( 0xFF19 ,0xBF ); 
    mem_write( 0xFF1A ,0x7F ); 
    mem_write( 0xFF1B ,0xFF ); 
    mem_write( 0xFF1C ,0x9F ); 
    mem_write( 0xFF1E ,0xBF ); 
    mem_write( 0xFF20 ,0xFF ); 
    mem_write( 0xFF21 ,0x00 ); 
    mem_write( 0xFF22 ,0x00 ); 
    mem_write( 0xFF23 ,0xBF ); 
    mem_write( 0xFF24 ,0x77 ); 
    mem_write( 0xFF25 ,0xF3 ); 
    mem_write( 0xFF26 ,0xF1 );
    mem_write( 0xFF40 ,0x91 ); 
    mem_write( 0xFF42 ,0x00 ); 
    mem_write( 0xFF43 ,0x00 ); 
    mem_write( 0xFF45 ,0x00 ); 
    mem_write( 0xFF47 ,0xFC ); 
    mem_write( 0xFF48 ,0xFF ); 
    mem_write( 0xFF49 ,0xFF ); 
    mem_write( 0xFF4A ,0x00 ); 
    mem_write( 0xFF4B ,0x00 ); 
    mem_write( 0xFFFF ,0x00 ); 

    mem.DMA_active = 0;
}


uint8_t mem_loadRom(char* path)
{
     
    FILE *romFile = fopen( path , "r");

    if (romFile == NULL)
    {
        printf("\nNo imput file found!\n\n");
        return 1;
    }

    // Get length of given File and allocate space
    fseek(romFile, 0, SEEK_END);
    mem.loadedRomLength = ftell(romFile);
    rewind(romFile);  

    mem.loadedRom = malloc(mem.loadedRomLength); 
    fread(mem.loadedRom, mem.loadedRomLength, 1, romFile);
    fclose(romFile);

    // get cartridge header information
    mem.cartType = mem.loadedRom[0x147];
    mem.cartRomBankCount = 1 << ( mem.loadedRom[0x148] + 1);
    mem.activeRomBank = 1;


    // TODO add MBC table to set function poitner to memory bank functions
    _setRomBank = &mem_setRomBankMBC1;
    // Allocate external Ram if any
    // TODO load external ram file .sav
    if (mem.loadedRom[0x149] < 0x06 && mem.loadedRom[0x149] > 1)
    {
        mem.cartRamBankCount = 1 << (mem.loadedRom[0x149] -1);
        mem.eRam = malloc(mem.cartRamBankCount * 0x2000);   
        mem.activeRamBank = 0;   
        
    }
    else
    {
        mem.cartRamBankCount = 0;
        mem.eRam = NULL;
    }

    #ifdef DEBUG
    printf("Succesfully loaded ROM '%s'\nloaded size %d bytes\n", path, mem.loadedRomLength );
    printf("External Ram detected: %s  - RAM Size 0x%X\n\n", (mem.cartRamBankCount != 0 ? "YES" : "NO"), 0x2000 * mem.cartRamBankCount);
    #endif

    return 0;

}

void mem_close(void)
{
    free(mem.loadedRom);
    free(mem.eRam);
}

// memory read function
uint8_t mem_read(uint16_t adr)
{
    switch (adr)
    {
        case 0x0000 ... 0x3FFF: // standard rom
            return mem.loadedRom[adr];
            break;


        case 0x4000 ... 0x7FFF:  // banked rom
            return mem.loadedRom[adr + (0x4000 * (mem.activeRomBank-1) )];
            break;


        case 0x8000 ... 0x9FFF:  // vram
        return mem.vRam [adr - 0x8000];
            break;


        case 0xA000 ... 0xBFFF:  // banked vram
            if (mem.cartRamBankCount != 0)
            {
                adr -= 0x4000;
                return mem.eRam[ adr + (0x2000 * (mem.activeRamBank))];
            }
            else
            {
                return 0;
            }
            break;


        case 0xC000 ... 0xCFFF:  // wram
            return mem.fixedRam[ adr - 0xC000];
            break;


        case 0xD000 ... 0xDFFF:  // schwitchable wRam
            return mem.switchedRam[0][ adr - 0xD000];
            break;


        case 0xE000 ... 0xFDFF:  // prohibited
            break;


        case 0xFE00 ... 0xFE9F:  // oam table
            return mem.oam[adr & 0xFF];
            break;


        case 0xFEA0 ... 0xFEFF:  // unusable
            break;
        case 0xFF00 ... 0xFF7F:  //io
            switch (adr)
                {
                    case 0xFF05: // TIMA
                        return mem.TIMA; 
                        break;
                    default:
                        return mem.io[adr & 0xFF];
                        break;
                }        


        case 0xFF80 ... 0xFFFE:  // high Ram
            return mem.hRam[adr - 0xFF80];
            break;


        case 0xFFFF:             //irq
            return mem.IE;
            break;


        default:
            break;
    }

    return 0;
}

// memory write function
uint8_t mem_write(uint16_t adr, uint8_t val)
{

    switch (adr)
    {
        case 0x0000 ... 0x1999:  // eRam Enable 0x0A = Enable        
            break;


        case 0x2000 ... 0x7FFF:  // Rom bank switching 
            _setRomBank(adr, val); 
            break;


        case 0x8000 ... 0x9FFF:  // VRAM
            mem.vRam[ adr - 0x8000] = val;
            break;


        case 0xA000 ... 0xBFFF:  // External RAM switching TODO!
            break;


        case 0xC000 ... 0xCFFF:
            mem.fixedRam[ adr - 0xC000] = val;
            break;


        case 0xD000 ... 0xDFFF: // TODO RAM SWITCHING
            mem.switchedRam[0][ adr - 0xD000] = val;
            break;


        case 0xE000 ... 0xFDFF:  // mirror ram
            break;


        case 0xFE00 ... 0xFE9F: // Sprite oam
            mem.oam[ adr & 0xFF ] = val;
            break;


        case 0xFEA0 ... 0xFEFF:  // Not Usable   
            break;


        case 0xFF00 ... 0xFF7F:  // IO Registers 
            switch (adr)
            {
                case 0xFF04: // DIV is 0 on write
                    mem.DIV = 0;
                    cpu._DIVhelper = cpu._TIMAhelper = 0;
                    break;  
                case 0xFF05:
                    mem.TIMA = val; // set TIMA and reset Helper
                    break;
                case 0xFF07:
                    cpu_setTAC(val);  // set TIMA prescaler/ synch timers
                    break;
                case 0xFF46: // set DMA_Active Flag on write to 0xFF46;
                    mem.DMA = val;
                    mem.DMA_active = 1; 
                    break;
                

                default:  
                    mem.io[ adr & 0xFF ] = val;
                    break;
                

            }
            break;

        case 0xFF80 ... 0xFFFE:  // HIGH Ram

            mem.hRam[ adr-0xFF80 ] = val;
            break;


        case 0xFFFF:            // IRQ Register
            mem.IE = val;
            break;


        default:
            break;
    }

    return 0;
}

// DMA handler -> happens instantly, timing probalby needs some tuning
// copys 159 bytes into oam Ram, specified by DMA register (specify upper 2 bytes of address. for example DMA = 0xEA -> adress start = 0xEA00) 
void mem_doDMA(void)
{
    uint16_t sourceAdr = (mem.DMA << 8)  | 0x0000; 
    uint16_t targetAdr = 0xFE00;

    for (uint8_t i = 0x00; i <= 0x9F; i++)
    {
        mem_write(targetAdr + i, mem_read(sourceAdr + i));
    }
    mem.DMA_active = 0;

}

// 5bit 0x01 - 0x1F - highe rbits discarded
// if val is higher then max number of banks- max bankis selected
// if more than 16 banks reg 0x4000-7FFF supplies 2 moire upper bits
// rombank = secBank << 5 + rombanks
// cannot be 0 -> 1
// if RomBank & 0xF = 0-> rombank+1

// MBC1 (cartridge) rom bank switcher
void mem_setRomBankMBC1(uint16_t adr, uint8_t val){


    if (adr <= 0x3FFF){ 
        // discard upper bits+ if valu bigger tahn max number - discard upper bits
        val &= (mem.cartRomBankCount -1);
        mem.activeRomBank &= ~(0x1F);
        mem.activeRomBank |= val; 
    }

    else if (mem.cartRomBankCount > 0x1F)
    {
        // set upper bits
        mem.activeRomBank &= ~(0x60);
        mem.activeRomBank |= (val << 5) & 0x60; 
    }

    // if selected rombank is higher than max rombank, select max
    //if ( (mem.activeRomBank & 0x6F) > mem.cartRomBankCount){
    //    mem.activeRomBank = mem.cartRomBankCount & 0x6F;
    //}
    // if lower bits are zero, increment by 1
    if ( (mem.activeRomBank & 0x0F) == 0){
        mem.activeRomBank += 1;
    }
}













