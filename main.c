#include <gb.h>



int main (int argc, char* argv[]) 
{
    
    if (mem_loadRom(argv[1]))
    {
        return 0;
    }

    
    printf("Hello GB \n");

    cpu_t c;
    c.HC = 1;
    printf("AF Register %x\n", c.AF);

    cpu_init();
    mem_init();
    ppu_init();
    mem_close();
 
    return 0;
    

} 