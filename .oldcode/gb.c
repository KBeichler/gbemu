#include "gb.h"
#include "opcode.h"


char debugfile[] = "/home/kev/DEV/EMU/gameboy/misc/bgbtest.gb";
char debugfile2[] = "D:\\Documents\\DEV\\old\\EMU\\gameboy\\misc\\bgbtest.gb";
uint8_t IRQ_TABLE[5] = { 0x40, 0x48, 0x50, 0x58, 0x60};


cpu_t * init_cpu(){

    cpu_t *cpu = malloc(sizeof(cpu_t));
    if (cpu == NULL){
        printf("Could not allocate memory\n");
        return 0;
    }


    memset((void *)cpu, 0, sizeof(cpu_t));
    memcpy(cpu->mem.boot_rom, BIOS, sizeof(BIOS));

    cpu->irq_enable = 1;        
    
    cpu->reg.PC = 0x100;
    cpu->reg.SP = 0xFFFE;
    cpu->cycle = 0;

    fake_powerup(cpu);
   
    return cpu;
}


uint8_t load_rom(cpu_t *cpu, char* path){
    uint8_t *mem = cpu->mem.data;
    uint32_t filelen; 

    #ifdef DEBUG
    FILE *rom = fopen( debugfile2 , "r");
    #else
    FILE *rom = fopen( path , "r");
        #endif 
    if (rom == NULL){
        return 0;
    }

    fseek(rom, 0, SEEK_END);
    filelen = ftell(rom);
    rewind(rom);  
    fread(mem, filelen, 1, rom);
    fclose(rom);
    #ifdef DEBUG
    printf("\n\nLOADED ROM FILE: %s\n", debugfile);
    #endif
    return 1;

}




void tick_cpu(cpu_t *cpu){

    // check interrupts
    if (cpu->irq_enable == 1)
    {
        for (uint8_t i = 0; i < 5 && cpu->irq_enable; i++){
            if ( ( MEM(I_ENABLE) & (1 << i)) && ( MEM(I_FLAG) & (1 << i)) ){

                PUSH(REG(PC));
                REG(PC) = IRQ_TABLE[i];
                //clear flag
                MEM(I_FLAG) &= ~( 1 << i );
                DISABLE_IRQ;
            }
        }


    }

    // check for DMA data transfer

    // fetch and run opcode
    run_Op(cpu);



    // update timer




}


void fake_powerup(cpu_t *cpu){
    //MEM(0xFF00) = 0xFF;
    REG(AF) = 0x01B0;
    cpu->reg.flags.R = 0xB0;
    REG(BC) = 0x0013;
    REG(DE) = 0x00D8;
    REG(HL) = 0x014D;
    REG(SP) = 0xFFFE;
    
   MEM(0xFF05) = 0x00   ; 
   MEM(0xFF06) = 0x00   ; 
   MEM(0xFF07) = 0x00   ; 
   MEM(0xFF10) = 0x80   ; 
   MEM(0xFF11) = 0xBF   ; 
   MEM(0xFF12) = 0xF3   ; 
   MEM(0xFF14) = 0xBF   ; 
   MEM(0xFF16) = 0x3F   ; 
   MEM(0xFF17) = 0x00   ; 
   MEM(0xFF19) = 0xBF   ; 
   MEM(0xFF1A) = 0x7F   ; 
   MEM(0xFF1B) = 0xFF   ; 
   MEM(0xFF1C) = 0x9F   ; 
   MEM(0xFF1E) = 0xBF   ; 
   MEM(0xFF20) = 0xFF   ; 
   MEM(0xFF21) = 0x00   ; 
   MEM(0xFF22) = 0x00   ; 
   MEM(0xFF23) = 0xBF   ; 
   MEM(0xFF24) = 0x77   ; 
   MEM(0xFF25) = 0xF3   ; 
   MEM(0xFF26) = 0xF1   ;
   MEM(0xFF40) = 0x91   ; 
   MEM(0xFF42) = 0x00   ; 
   MEM(0xFF43) = 0x00   ; 
   MEM(0xFF45) = 0x00   ; 
   MEM(0xFF47) = 0xFC   ; 
   MEM(0xFF48) = 0xFF   ; 
   MEM(0xFF49) = 0xFF   ; 
   MEM(0xFF4A) = 0x00   ; 
   MEM(0xFF4B) = 0x00   ; 
   MEM(0xFFFF) = 0x00   ; 


















}