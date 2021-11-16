#include <gb.h>


extern cpu_t cpu;
extern mem_t mem;
extern ppu_t ppu;

char * testfile = "./cpu_instrs1.gb";

void cputest_debugPrint(){


    printf("------------------------------------------------------------\n");
    printf("CPU-Clock: %13d\tPPU-clock: %13d\nOpCode: 0x%02X\t+1: 0x%02X\t+2: 0x%02X\n",cpu.clock, ppu.clock, mem_read(cpu.PC), mem_read(cpu.PC+1), mem_read(cpu.PC+2));
    printf("Registers:\nAF: 0x%02X\tBC: 0x%02X\tDE: 0x%02X\t HL: 0x%02X\nPC: 0x%02X\tSP: 0x%02X", cpu.AF, cpu.BC, cpu.DE, cpu.HL,  cpu.PC, cpu.SP);
    printf("\t FLAGS:   Z:%d N:%d HC:%d C:%d\n", !!(cpu.F & 1<<7), !!(cpu.F & 1<<6), !!(cpu.F & 1<<5), !!(cpu.F & 1<<4) );
    printf("Memory:\n@SP: 0x%02X\t+1: 0x%02X\t -1 0x%02X\t-2 0x%02X", mem_read(cpu.SP),mem_read(cpu.SP+1),mem_read(cpu.SP-1), mem_read(cpu.SP-2));
    printf("\n@HL: 0x%02X\t +1 0x%02X\n", mem_read(cpu.HL), mem_read(cpu.HL+1));
    printf("IO Registers:\n");
    printf("IF: 0x%02X\tIE: 0x%02X\tLCDC: 0x%02X\tSTAT: 0x%02X\n", mem.IF, mem.IE, mem.LCDC, mem.STAT);
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
}


// status: we run ok at leas until Px 0x763 

int main(void){
    cpu_init();
    ppu_init();
    mem_init();
    mem_loadRom(testfile);
    cputest_fakepowerup();
    cputest_debugPrint();


    printf("CPU Test with blargs opcode test\n");
    printf("video is currently not supported\n");
    printf("Start test with rom file %s and init Values:\n", testfile);
    

    uint8_t run = 0;
    uint8_t singlestep = 0;
    uint32_t breakpoint;
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
        if (scanf( "%x", &breakpoint ) == 1)
        {
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
        while (ppu.clock < cpu.clock)
        {
            ppu_tick();
        }


        if (cpu.PC == breakpoint || singlestep)
        {
            cputest_debugPrint();
            
           
            if (!singlestep)
            {
                printf("Next Breakpoint PC: ");                
                if (scanf( "%x", &breakpoint ) == 1)
                {
                    run = 1;
                }
            }
            else
            {
                printf("Abort with y: ");
                if (getchar() == 'y') run = 0;
            }

        }
    }

    return 0; 
} 