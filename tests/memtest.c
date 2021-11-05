#include <gb.h>


int main (int argc, char* argv[]) 
{
    
    if (mem_loadRom(argv[1]))
    {
        printf("No Args supplied\n");
        return 0;
    }

    
    printf("Hello GB Test \n");,

    mem_init();
    char * romFile = "../tests/testfiles/mbc1.test" ;
    mem_loadRom(romFile);
    mem_close();
 
    return 0;
    

} 