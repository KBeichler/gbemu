#ifndef CPU
#define CPU

#include <gb.h>



/*
     _       _              _       __ _       _ _   _                 
  __| | __ _| |_ __ _    __| | ___ / _(_)_ __ (_) |_(_) ___  _ __  ___ 
 / _` |/ _` | __/ _` |  / _` |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
| (_| | (_| | || (_| | | (_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 \__,_|\__,_|\__\__,_|  \__,_|\___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/
                                                                     
*/

// Structure to hold all Registers aswell as status infomraiton about the CPU
typedef struct cpu_t
{

    // Data Registers
    // for easier handling, each 16 bit register is set as a union -> 1 16bit and 2 8bit values
    // except for Register AF -> Flags are a seperate union
    union 
    {
        uint16_t BC;
        struct
        {
            uint8_t C;
            uint8_t B;
        };
    };

    union 
    {
        uint16_t DE;
        struct
        {
            uint8_t E;
            uint8_t D;
        };
    };

    union 
    {
        uint16_t HL;
        struct
        {
            uint8_t L;
            uint8_t H;
        };
    };

    union 
    {
        uint16_t AF;
        struct
        {
            uint8_t F;
            struct
            {
                uint8_t _n :4;
                uint8_t CY :1;
                uint8_t HC :1;
                uint8_t N :1;
                uint8_t Z :1;
            };
            uint8_t A;
        };
    };
    // Full 16 bit regisrers
    uint16_t SP;
    uint16_t PC;   


    // Others
    uint8_t irq_enable;
    uint32_t clock;

}cpu_t;


// Register Numbers for opcode decoding
typedef enum regNum_e
{
    B = 0,
    C,
    D,
    E,
    H,
    L,
    HL,
    A
}regNum_e;




void cpu_init(void);



















#endif