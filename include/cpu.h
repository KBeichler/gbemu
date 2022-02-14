/*  cpu.h
*   used in the gb-emu project. cpu.h and cpu.c are needed for the basic function of the cpu emulations
*   
*   contains:
*       (nearly) all opcodes as preprocessor definitons
*       defintions for functions
*       defintions of the cpu_t struct
*
*
*   
*
*/


#ifndef CPU
#define CPU

#include <gb.h>


// OPCODE ACCES FUNCTIONS AND HELPER VARIABLES
uint8_t i;
uint16_t n;
int8_t s;
#define REG(r) cpu.r
#define FLAG(f) cpu.f


#define ARG_1(D)  (( D & 0x38 ) >> 3) 
#define ARG_2(D)  (( D & 0x07 )     )



/*
  ___         ____          _
 / _ \ _ __  / ___|___   __| | ___  ___
| | | | '_ \| |   / _ \ / _` |/ _ \/ __|
| |_| | |_) | |__| (_) | (_| |  __/\__ \
 \___/| .__/ \____\___/ \__,_|\___||___/
      |_|
*/

// LD_R_R
// D Destination, S source
#define LD_R_n(D, S)    D = S
#define LD_RR_D16(D)    D =  mem_read(REG(PC++)); D |= ( uint16_t )mem_read(REG(PC++))  << 8 ;
#define LD_MEM_n(D, S)  mem_write(D, S);

// 8 BIT ARITHMETIC
// S = Register
#define ADD_R(S)        i = S; FLAG(HC) =  ((REG(A) & 0xF) + (i & 0xF)) > 0xF;  REG(A) += i; FLAG(Z) = (REG(A) == 0); FLAG(N) = 0; FLAG(CY) = (REG(A) - i) < 0x00; 
#define SUB_R(S)        i = S; FLAG(HC) =   (REG(A) & 0xF) < (i & 0xF) ;        REG(A) -= i; FLAG(Z) = (REG(A) == 0); FLAG(N) = 1; FLAG(CY) = (REG(A) + i) > 0xFF; 
#define ADC_R(S)        i = S; n = FLAG(CY); FLAG(HC) =  ((REG(A) & 0xF) + (i & 0xF) +n )  > 0xF;  REG(A) = REG(A) + i + n; FLAG(Z) = (REG(A) == 0); FLAG(N) = 0; FLAG(CY) = (REG(A) - i-n) < 0x00;
#define SBC_R(S)        i = S; n = FLAG(CY); FLAG(HC) =   (REG(A) & 0xF) < ((i & 0xF) +n)       ;  REG(A) = REG(A) - i - n; FLAG(Z) = (REG(A) == 0); FLAG(N) = 1; FLAG(CY) = (REG(A) + i+n) > 0xFF;
#define AND_R(S)       FLAG(N) = FLAG(CY) = 0; FLAG(HC) = 1; REG(A) &= S; FLAG(Z) = (REG(A) == 0);  
#define XOR_R(S)       FLAG(N) = FLAG(CY) =    FLAG(HC) = 0; REG(A) ^= S; FLAG(Z) = (REG(A) == 0);  
#define OR_R(S)        FLAG(N) = FLAG(CY) =    FLAG(HC) = 0; REG(A) |= S; FLAG(Z) = (REG(A) == 0);  
#define CP_R(S)        i = S; FLAG(N) = 1; FLAG(Z) = ( REG(A) == i ); FLAG(CY) = REG(A) < i; FLAG(HC) = ((REG(A) & 0xF) < (i & 0xF)); 

#define INC_R(S)       FLAG(N) = 0; FLAG(HC) = (S & 0xF) == 0xF; S++; FLAG(Z) = S == 0; 
#define DEC_R(S)       FLAG(N) = 1; FLAG(HC) = (S & 0xF) == 0x0; S--; FLAG(Z) = S == 0; 

#define INC_HL       i = mem_read(REG(HL)); FLAG(N) = 0; FLAG(HC) = (i & 0xF) == 0xF; i++; FLAG(Z) = i == 0; mem_write(REG(HL), i );
#define DEC_HL       i = mem_read(REG(HL)); FLAG(N) = 1; FLAG(HC) = (i & 0xF) == 0x0; i--; FLAG(Z) = i == 0; mem_write(REG(HL), i );


// 16 BIT ARITHEMITC
// S = Register
#define INC_RR(S)      S++;  
#define DEC_RR(S)      S--;  
#define ADD_RR(S)     FLAG(N) = 0; n = S; FLAG(HC) = ( (S & 0x0FFF) + ( REG(HL) & 0x0FFF) ) > 0x0FFF; REG(HL) += S; FLAG(CY) = (REG(HL) - n) < 0x0;   
// SPECIAL 16 BIT ARITHEMTIC
#define LDHL        FLAG(Z) = FLAG(N) = 0; s = (int8_t) mem_read(REG(PC)++); n = REG(SP) + s; FLAG(HC) = ( (n & 0xF) < (s & 0xF)) ; FLAG(CY) = ( (n & 0xFF)<(s & 0xFF) );  REG(HL) = n; 
#define ADD_SP      FLAG(Z) = FLAG(N) = 0; s = (int8_t) mem_read(REG(PC)++); n = REG(SP) + s; FLAG(HC) = ( (n & 0xF) < (s & 0xF)) ; FLAG(CY) = ( (n & 0xFF)<(s & 0xFF) );  REG(SP) = n; 

// JMP AND CALL 
// F = Condition to executssssss
#define JMP_n(F)       i = mem_read(REG(PC++)); if (F) { REG(PC) += (int8_t) i; cpu.currentCycleLength  += 1;} 
#define JMP_nn(F)      n = mem_read(REG(PC++)); n |= (mem_read(REG(PC++)) << 8); if (F) { REG(PC) = n; cpu.currentCycleLength  += 1; } ;
#define CALL(F)        n = mem_read(REG(PC++)); n |= (mem_read(REG(PC++)) << 8); if (F) { PUSH(REG(PC)); REG(PC) = n; cpu.currentCycleLength  += 3; }
#define RET(F)         if (F) { POP(REG(PC)); cpu.currentCycleLength += 3; }
#define RST(I)         PUSH( REG(PC) );  REG(PC) = 0x0000 + I;           

// SHIFT OPERATIONS
#define RLC(S)          FLAG(N) = FLAG(HC) = 0; FLAG(CY) = !!(S & 0x80) ; S = (S << 1) + (FLAG(CY) > 0) ; FLAG(Z) = S == 0;	
#define RL(S)           FLAG(N) = FLAG(HC) = 0; short f  = S >> 7       ; S = (S << 1) | FLAG(CY)       ; FLAG(Z) = S == 0;	FLAG(CY) = !!f;  
#define RRC(S)          FLAG(N) = FLAG(HC) = 0; FLAG(CY) = S & 0x01     ; S = (S >> 1) | (FLAG(CY) << 7); FLAG(Z) = S == 0;	
#define RR(S)           FLAG(N) = FLAG(HC) = 0; short f  = S & 0x01     ; S = (S >> 1) | (FLAG(CY) << 7); FLAG(Z) = S == 0; FLAG(CY) = !!f;	 
#define SLA(S)          FLAG(N) = FLAG(HC) = 0; FLAG(CY) = !!(S  & 0x80); S = S << 1                    ; FLAG(Z) = S == 0;	
#define SRA(S)          FLAG(N) = FLAG(HC) = 0; FLAG(CY) = S & 0x01     ; S = (S & 0x80) | (S >> 1)     ; FLAG(Z) = S == 0;	
#define SRL(S)          FLAG(N) = FLAG(HC) = 0; FLAG(CY) = S & 0x01     ; S = (S >> 1)                  ; FLAG(Z) = S == 0;	
#define SWAP(S)         FLAG(N) = FLAG(CY) =    FLAG(HC) = 0            ; S = (S >> 4) | (S << 4)       ; FLAG(Z) = S == 0;  
#define RES(B , S)      S &= ~( 1 << B );  
#define SET(B , S)      S |=  ( 1 << B );  
#define BIT(B , S)      FLAG(N) = 0; FLAG(HC) = 1; FLAG(Z) = !( ( S & (1 << B)) );  

// STACK OPERATIONS
#define PUSH(S)        mem_write(--REG(SP),(uint8_t) (S >> 8)); mem_write(--REG(SP), (uint8_t) (S & 0xFF)); 
#define POP(D)         D = mem_read(REG(SP)++); D |= mem_read(REG(SP)++) << 8;        


// IRQ HELPERSs
#define ENABLE_IRQ     cpu.irq_enable = 1;
#define DISABLE_IRQ    cpu.irq_enable = 0;
#define TRIGGER_IRQ(I) { mem.IF |= ( 1 << I); }


/*
     _       _              _       __ _       _ _   _                 
  __| | __ _| |_ __ _    __| | ___ / _(_)_ __ (_) |_(_) ___  _ __  ___ 
 / _` |/ _` | __/ _` |  / _` |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
| (_| | (_| | || (_| | | (_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 \__,_|\__,_|\__\__,_|  \__,_|\___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/
                                                                     
*/

// main cpu struct: Structure to hold all Registers aswell as status information about the CPU
typedef struct cpu_t
{

    // Data Registers
    // for easier handling, each 16 bit register is set as a union -> one 16bit and two 8bit values
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

    union                   // AF is a untion of the 8 bit A Register and the 8 bit B register, accesible as bitfield for every flag
    {
        uint16_t AF;
        struct
        {
            union
            {
                uint8_t F;
                struct
                {
                    uint8_t _n :4;
                    uint8_t CY :1;  // Carry Flag
                    uint8_t HC :1;  // Half Carry Flag
                    uint8_t N :1;   // N Flag
                    uint8_t Z :1;   // Zero Flag
                };
            };
            uint8_t A;
        };
    };
    // Full 16 bit registers
    uint16_t SP;    // stack pointer
    uint16_t PC;    //program coutner


    // internal status variables
    uint8_t irq_enable;
    uint32_t clock;
    uint8_t currentCycleLength;
    uint8_t prefixCode;
    uint8_t halt;

    // Helper variables
    uint16_t _DIVhelper;
    uint16_t _TIMAhelper;
    uint16_t _TIMAdivider;

}cpu_t;


/* 
 _____                 _   _
|  ___|   _ _ __   ___| |_(_) ___  _ __
| |_ | | | | '_ \ / __| __| |/ _ \| '_ \
|  _|| |_| | | | | (__| |_| | (_) | | | |
|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|

     _       __ _       _   _
  __| | ___ / _(_)_ __ | |_(_) ___  _ __  ___
 / _` |/ _ \ |_| | '_ \| __| |/ _ \| '_ \/ __|
| (_| |  __/  _| | | | | |_| | (_) | | | \__ \
 \__,_|\___|_| |_|_| |_|\__|_|\___/|_| |_|___/

 */

/* void cpu_init(void)
    initializes the cpu with standard values
    takes no arguments as all variables are global (lol)

*/
void cpu_init(void);

/* void cpu_tick(void)
    main cpu function
    2022.02.13: tick function. takes next argument from $PC and exectues the opcode
        also-> checks for interrups. if IRQ is pending, PC gets pushed on stack and IRQ Vecot gets loaded into PC
            
*/
void cpu_tick(void);

/* void cpu_updateTimer(uint8_t currentClock)
    updates the DIV and TIMA timer Registers. Currentclock ist the duraton of the last executed opcode. saved in cpu.currentclockCycle
*/
void cpu_updateTimer(uint8_t currentClock);

/* void DAA(void)
    handels the DAA opcodes (0x27) becuase its quite long. al values get saved into global variables
*/
void DAA(void);


void cpu_setTAC(uint8_t val);















































































































































































#endif