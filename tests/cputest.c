#include <gb.h>

extern cpu_t cpu;
extern mem_t mem;
uint8_t test_values[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

int main(void){
    printf("Hello CPU Test\n");
    mem.loadedRom = malloc(sizeof(uint8_t) * 0x1000);
    cpu.PC = 0x00;


    printf("TEST LD B C: \n");
    mem.loadedRom[0x00] = 0x41;
    cpu.C = 5;
    printf("CPU.C %x CPU.B %x\nTick\n", cpu.C, cpu.B);
    cpu_tick();
    printf("CPU.C %x CPU.B %x Cpu.PC %x\n", cpu.C, cpu.B, cpu.PC);

    printf("Test PUSH… BC = 0x%x\n", cpu.BC);
    cpu.SP = 0xE000;
    cpu.BC = 0x123;
    mem.loadedRom[0x01] = 0xC5;
    cpu_tick();
    printf("cpu.SP %x value 1 %x value 2 %x\n", cpu.SP, mem_read(cpu.SP), mem_read(cpu.SP+1));
    cpu.BC = 0;
    
    mem.loadedRom[0x02] = 0xD1;
    cpu_tick();
    printf("POP DE:  CPU.DE %x\n", cpu.DE);



    free(mem.loadedRom);
    return 0;
} 