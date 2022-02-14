/*
    Debugging helper program
    its pretty shit
    but it works kind of
*/
#include <gb.h>
#include <dissass.h>

extern cpu_t cpu;
extern mem_t mem;
extern ppu_t ppu;

char * testfile = "/media/storage/Documents/DEV/projects/gbemu/builds/cpu_instrs1.gb";
//char * testfile = "/mnt/d/Documents/DEV/projects/gbemu/builds/cpu_instrs1.gb";
uint8_t run = 0;



void cputest_debugPrint(){

    char *currentcode = cpu.prefixCode == 0 ? dissas_opcodename[mem_read(cpu.PC)] : dissas_exopcodename[mem_read(cpu.PC)];

    printf("------------------------------------------------------------\n");
    printf("OpCode: 0x%02X   %s\t+1: 0x%02X\t+2: 0x%02X\n",mem_read(cpu.PC), currentcode, mem_read(cpu.PC+1), mem_read(cpu.PC+2));
    printf("CPU-Clock: %13d\tPPU-clock: %13d\t last cycle %d\n",cpu.clock, ppu.clock, cpu.currentCycleLength);
    printf("Registers:\nAF: 0x%02X\tBC: 0x%02X\tDE: 0x%02X\t HL: 0x%02X\nPC: 0x%02X\tSP: 0x%02X", cpu.AF, cpu.BC, cpu.DE, cpu.HL,  cpu.PC, cpu.SP);
    printf("\t FLAGS:   Z:%d N:%d HC:%d C:%d\n", !!(cpu.F & 1<<7), !!(cpu.F & 1<<6), !!(cpu.F & 1<<5), !!(cpu.F & 1<<4) );
    printf("Memory:\n@SP: 0x%02X\t+1: 0x%02X\t -1 0x%02X\t-2 0x%02X", mem_read(cpu.SP),mem_read(cpu.SP+1),mem_read(cpu.SP-1), mem_read(cpu.SP-2));
    printf("\n@HL: 0x%02X\t +1 0x%02X\n", mem_read(cpu.HL), mem_read(cpu.HL+1));
    printf("IO Registers:\t\t IRQ_Enable: %x\n", cpu.irq_enable); 
    printf("IF: 0x%02X\tIE: 0x%02X\tLCDC: 0x%02X\tSTAT: 0x%02X\n", mem.IF, mem.IE, mem.LCDC, mem.STAT);
    printf("SCY: 0x%X\t SCX: 0x%X\tLY: 0x%X\n", mem.SCY, mem.SCX, mem.LY);
    printf("DIV: 0x%X\t TIMA: 0x%X\tTMA: 0x%X\thelper %d\tdivider: 0%d\n", mem.DIV, mem.TIMA, mem.TMA, cpu._TIMAhelper, cpu._TIMAdivider);
    //printf("hRam: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", mem.hRam[0], mem.hRam[1], mem.hRam[2], mem.hRam[3], mem.hRam[4], mem.hRam[5]);
    printf("------------------------------------------------------------\n");

}
void flush(FILE* in)
{
   int c;
   while ( (c = fgetc(in)) != EOF && c != '\n');
}

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
    mem.DIV = 0x1E;
    cpu._DIVhelper = 0x1e00;
}
// status: we run ok at leas until Px 0x2d2 
 
 

int main (int argc, char* argv[])
{
    
    uint32_t breakpoint;
    char  input[32];

    uint8_t singlestep = 0;
    FLAG(HC) = ( (n & 0xF) < (s & 0xF)) ; FLAG(CY) = ( (n & 0xFF)<(s & 0xFF) );

    printf("0x%x \n", singlestep);
    cpu_init();
    ppu_init();
    mem_init();
    window_init(); 

    if (argc < 2)
    {
        if (mem_loadRom(testfile)) return 0;
    }
    else 
    {
        if (mem_loadRom(argv[1])) return 0;
    }
    
    cputest_fakepowerup();
    cputest_debugPrint();

    printf("CPU Test with blargs opcode test\n");
    printf("Start test with rom file %s and init Values:\n", testfile);
    


    printf("Singlestep on? Y/y\n");
    char in = getchar();
    if (in == 'Y' || in == 'y')
    {
        singlestep = 1;
        run = 1;
    }
    while ((getchar()) != '\n');
    if (!singlestep){
        printf("Set Breakpoint PC: ");
        if (scanf( "%s", input ) >= 1)
        {
            breakpoint = strtoul(input, NULL, 16);
            run = 1;
        }
        else
        {
            printf("Invalid input, aborting...\n");
        }   
    }

    
    while (run)
    {
        
        cpu_tick();
        while (ppu.clock <  ( cpu.clock >> 1) )
        {
            ppu_tick();
        }
        if (mem.DMA_active) { mem_doDMA(); mem.DMA_active = 0; }

        // blarggs serial output        
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

        if (cpu.PC == breakpoint || singlestep )
        {
            cputest_debugPrint();
            
           
            if (!singlestep)
            {
                printf("Next Breakpoint PC: ");    

                if ( scanf( "%s", input ) >= 1 )
                {
                    run = 1;
                    if ( input[0] == 's' ) singlestep = 1;
                    else breakpoint = strtoul(input, NULL, 16);
                }

            }
            else
            {
                printf("Abort with y, cont with n, <n> for breakpoitn ");
                scanf( "%s", input );
                if (input[0] == 'y') run = 0;
                else if (input[0] == 'n');
                else
                {
                    breakpoint = strtoul(input, NULL, 16);
                    singlestep = 0;
                } 
            }
        }
        window_getIO(&run);
    }
    
    window_close();
    mem_close(); 

    return 0; 
} 