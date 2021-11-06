#define DEBUG
#include <mem.h>
#include <stdint.h>
#include <stdlib.h>



extern mem_t mem;
/*
char * mbc1testfile = "../tests/testfiles/mbc1.test";
char * mbc2testfile = "../tests/testfiles/mbc2.test";
char * mbc3testfile = "../tests/testfiles/mbc3.test";
char * mbc4testfile = "../tests/testfiles/mbc4.test";
char * mbc5testfile = "../tests/testfiles/mbc5.test";*/

char * testfiles[] = 
{
"../tests/testfiles/mbc1.gb",
"../tests/testfiles/mbc2.gb",
"../tests/testfiles/mbc3.gb",
"../tests/testfiles/mbc4.gb",
"../tests/testfiles/mbc5.gb",
"../tests/testfiles/tetris.gb"
};

int main (int argc, char* argv[]) 
{
    //setup romfile
    char *romfile;
    uint8_t mbcNr;
    if (argc < 2)
    {
        romfile = testfiles[0];
        mbcNr = 1;
    }
    else if (argc == 2)
    {
        mbcNr = atoi(argv[1]);
        romfile = testfiles[mbcNr];
    }
    else
    {
        printf("Invalid Args supplied\n");
        return 0;
    }
    
    mem_init();
    if ( mem_loadRom(romfile) )
    {
        printf("Testfile '%s' not found\n", romfile);
        return 0;
    }

    printf("Hello GB MEM Test \n");
    


    // beginn tests

    // mbc1
    printf("RomBankCount %d\n", mem.cartRomBankCount);
    uint16_t a = 0x1000;

    for (uint8_t i = 0; i <mem.cartRomBankCount+1; i++){
        printf("\nset Rombank to %d (max Rombank = %d)\n", i, mem.cartRomBankCount-1);
        mem_write(0x3000, i);
        printf("Active RomBank %d\n", mem.activeRomBank);
        printf(" 0x%x = 0x%x\t %x = 0x%x\t 0x%x = 0x%x\t 0x%x = 0x%x\t\n", a,  mem_read(a), a*3,  mem_read(a*3),a*4,  mem_read(a*4), a*6,  mem_read(a*6));
       
    }



    // end tests

    mem_close();
 
    return 0;
    
 
} 