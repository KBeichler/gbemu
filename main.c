#include <gb.h>




extern cpu_t cpu;
extern ppu_t ppu;
extern mem_t mem;
SDL_Event event;
void cputest_fakepowerup()
{

    cpu.AF = 0x01B0;
    cpu.PC = 0x100;
    cpu.BC = 0x0013;
    cpu.DE = 0x00D8;
    cpu.HL = 0x014D;
    cpu.SP = 0xFFFE;
    mem_write(0xFF05, 0x00)   ; 
    mem_write(0xFF06, 0x00)   ; 
    mem_write(0xFF07, 0x00)   ; 
    mem_write(0xFF10, 0x80)   ; 
    mem_write(0xFF11, 0xBF)   ; 
    mem_write(0xFF12, 0xF3)   ; 
    mem_write(0xFF14, 0xBF)   ; 
    mem_write(0xFF16, 0x3F)   ; 
    mem_write(0xFF17, 0x00)   ; 
    mem_write(0xFF19, 0xBF)   ; 
    mem_write(0xFF1A, 0x7F)   ; 
    mem_write(0xFF1B, 0xFF)   ;  
    mem_write(0xFF1C, 0x9F)   ; 
    mem_write(0xFF1E, 0xBF)   ; 
    mem_write(0xFF20, 0xFF)   ; 
    mem_write(0xFF21, 0x00)   ; 
    mem_write(0xFF22, 0x00)   ; 
    mem_write(0xFF23, 0xBF)   ; 
    mem_write(0xFF24, 0x77)   ; 
    mem_write(0xFF25, 0xF3)   ; 
    mem_write(0xFF26, 0xF1)   ;
    mem_write(0xFF40, 0x91)   ; 
    mem_write(0xFF42, 0x00)   ; 
    mem_write(0xFF43, 0x00)   ; 
    mem_write(0xFF45, 0x00)   ; 
    mem_write(0xFF47, 0xFC)   ; 
    mem_write(0xFF48, 0xFF)   ; 
    mem_write(0xFF49, 0xFF)   ; 
    mem_write(0xFF4A, 0x00)   ; 
    mem_write(0xFF4B, 0x00)   ; 
    mem_write(0xFFFF, 0x00)   ; 
    mem.IF = 0xE1;

}


char * testfile = "/media/storage/Documents/DEV/projects/gbemu/builds/cpu_instrs1.gb";
int main (int argc, char* argv[]) 
{
    cpu_init();
    mem_init();
    ppu_init();
    cputest_fakepowerup();
    printf(" argc %d\n", argc);
    if (argc < 2)
    {
        if (mem_loadRom(testfile)) return 0;
    }
    else 
    {
        if (mem_loadRom(argv[1])) return 0;
    }

    
    printf("Hello GB \n");


    //HELPERS

    uint8_t quit = 1;
    uint8_t lastCycle = 0;


    // SDL Main Windows Init
    window_init(); 

    while (quit){

        uint8_t lastCycle = cpu_tick();
        // update timers
        cpu_updateTimer( lastCycle );
        lastCycle *= 2; 
        while ( lastCycle-- )
        {
            ppu_tick();
        }

        if (mem.DMA_active == 1)
        {
            mem_doDMA();
            mem.DMA_active = 0;
        }

        // debug serial output        
        if (mem_read(0xff02) == 0x81) {

            char c = mem_read(0xff01);
            printf("%c", c);
            mem_write(0xff02,  0x0);
        }

        if (ppu.newFrame)
        {
            ppu.newFrame = 0;
            //draw screen
            // color from pallette for each pixel
            window_drawFrame(ppu.framebuffer);

 
        }
        
        
        window_getIO(&quit);


    }



    window_close();

    mem_close();
 
    return 0;
    

} 












void gb_fakepowerup()
{
    cpu.AF = 0x01B0;
    cpu.PC = 0x100;
    cpu.BC = 0x0013;
    cpu.DE = 0x00D8;
    cpu.HL = 0x014D;
    cpu.SP = 0xFFFE;
    mem_write(0xFF05, 0x00)   ; 
    mem_write(0xFF06, 0x00)   ; 
    mem_write(0xFF07, 0x00)   ; 
    mem_write(0xFF10, 0x80)   ; 
    mem_write(0xFF11, 0xBF)   ; 
    mem_write(0xFF12, 0xF3)   ; 
    mem_write(0xFF14, 0xBF)   ; 
    mem_write(0xFF16, 0x3F)   ; 
    mem_write(0xFF17, 0x00)   ; 
    mem_write(0xFF19, 0xBF)   ; 
    mem_write(0xFF1A, 0x7F)   ; 
    mem_write(0xFF1B, 0xFF)   ;  
    mem_write(0xFF1C, 0x9F)   ; 
    mem_write(0xFF1E, 0xBF)   ; 
    mem_write(0xFF20, 0xFF)   ; 
    mem_write(0xFF21, 0x00)   ; 
    mem_write(0xFF22, 0x00)   ; 
    mem_write(0xFF23, 0xBF)   ; 
    mem_write(0xFF24, 0x77)   ; 
    mem_write(0xFF25, 0xF3)   ; 
    mem_write(0xFF26, 0xF1)   ;
    mem_write(0xFF40, 0x91)   ; 
    mem_write(0xFF42, 0x00)   ; 
    mem_write(0xFF43, 0x00)   ; 
    mem_write(0xFF45, 0x00)   ; 
    mem_write(0xFF47, 0xFC)   ; 
    mem_write(0xFF48, 0xFF)   ; 
    mem_write(0xFF49, 0xFF)   ; 
    mem_write(0xFF4A, 0x00)   ; 
    mem_write(0xFF4B, 0x00)   ; 
    mem_write(0xFFFF, 0x00)   ; 
}