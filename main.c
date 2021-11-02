#include <gb.h>


int main(void){

    printf("Hello GB \n");

    cpu_t c;
    c.HC = 1;
    printf("AF Register %x\n", c.AF);
    cpu_init();

    return 0;

}