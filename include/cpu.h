#ifndef CPU
#define CPU

#include <gb.h>


// ACCESS FUNCTIONS
uint8_t i;
uint16_t n;
int8_t s;
#define REG(r) cpu.r

#define FLAG(f) cpu.f


#define ARG_1(D)  (( D & 0x38 ) >> 3) 
#define ARG_2(D)  (( D & 0x07 )     )

// LD_R_R
// D Destination, S source
#define LD_R_n(D, S)    D = S
#define LD_RR_D16(D)    D =  mem_read(REG(PC++)); D |= ( uint16_t )mem_read(REG(PC++))  << 8 ;
#define LD_MEM_n(D, S)  mem_write(D, S);






// 8 BIT ARITHMETIC
// S = Register
#define ADD_R(S)        i = S; FLAG(HC) =  ((REG(A) & 0xF) + (i & 0xF)) > 0xF;  REG(A) += i; FLAG(Z) = (REG(A) == 0); FLAG(N) = 0; FLAG(CY) = (REG(A) - i) < 0x00; 
#define SUB_R(S)        i = S; FLAG(HC) =   (REG(A) & 0xF) < (i & 0xF) ;        REG(A) -= i; FLAG(Z) = (REG(A) == 0); FLAG(N) = 1; FLAG(CY) = (REG(A) + i) > 0xFF; 
#define ADC_R(S)        i = S; n = FLAG(HC); FLAG(HC) =  ((REG(A) & 0xF) + (i & 0xF)) > 0xF;  REG(A) = REG(A) + i + n; FLAG(Z) = (REG(A) == 0); FLAG(N) = 0; FLAG(CY) = (REG(A) - i-n) < 0x00;
#define SBC_R(S)        i = S; n = FLAG(HC); FLAG(HC) =   (REG(A) & 0xF) < (i & 0xF);  REG(A) = REG(A) - i - n; FLAG(Z) = (REG(A) == 0); FLAG(N) = 1; FLAG(CY) = (REG(A) + i+n) > 0xFF;
#define AND_R(S)       FLAG(N) = FLAG(CY) = 0; FLAG(HC) = 1; REG(A) &= S; FLAG(Z) = (REG(A) == 0);  
#define XOR_R(S)       FLAG(N) = FLAG(CY) =    FLAG(HC) = 0; REG(A) ^= S; FLAG(Z) = (REG(A) == 0);  
#define OR_R(S)        FLAG(N) = FLAG(CY) =    FLAG(HC) = 0; REG(A) |= S; FLAG(Z) = (REG(A) == 0);  
#define CP_R(S)        i = S; FLAG(N) = 1; FLAG(Z) = ( REG(A) == i ); FLAG(CY) = REG(A) < i; FLAG(HC) = ((REG(A) & 0xF) < (i & 0xF)); 

#define INC_R(S)       FLAG(N) = 0; FLAG(HC) = (S & 0xF) == 0xF; S++; FLAG(Z) = S == 0; 
#define DEC_R(S)       FLAG(N) = 1; FLAG(HC) = (S & 0xF) == 0x0; S--; FLAG(Z) = S == 0; 

#define INC_HL       i = mem_read(REG(HL)); FLAG(N) = 0; FLAG(HC) = (i & 0xF) == 0xF; i++; FLAG(Z) = i == 0; mem_write(REG(HL), i );
#define DEC_HL       i = mem_read(REG(HL)); FLAG(N) = 1; FLAG(HC) = (i & 0xF) == 0x0; i--; FLAG(Z) = i == 0; mem_write(REG(HL), i );




#define LDHL        FLAG(Z) = FLAG(N) = 0; s = (int8_t) mem_read(REG(PC)++); FLAG(HC) = ( ( REG(SP) & 0x0FFF) + s) > 0x0FFF; REG(HL) = REG(SP) + s; FLAG(CY) = (REG(SP) - s - REG(SP)) < 0x0; 
#define ADD_SP      FLAG(Z) = FLAG(N) = 0; s = (int8_t) mem_read(REG(PC)++); FLAG(HC) = ( ( REG(SP) & 0x0FFF) + s) > 0x0FFF; REG(SP) += s; FLAG(CY) = (REG(SP) - s) < 0x0; 
// 16 BIT ARITHEMITC
// S = Register
#define INC_RR(S)      S++;  
#define DEC_RR(S)      S--;  
#define ADD_RR(S)     FLAG(N) = 0; FLAG(HC) = ( (S & 0x0FFF) + ( REG(HL) & 0x0FFF) ) > 0x0FFF; REG(HL) += S; FLAG(CY) = (REG(HL) - S) < 0x0;   

// JMP AND CALL 
// F = Condition to execut
#define JMP_n(F)       i = mem_read(REG(PC++)); if (F) { REG(PC) += (int8_t) i; cpu.currentCycleLength  += 3;} 
#define JMP_nn(F)      n = mem_read(REG(PC++)); n |= (mem_read(REG(PC++)) << 8); if (F) { REG(PC) = n; cpu.currentCycleLength  += 1; } ;
#define CALL(F)        n = mem_read(REG(PC++)); n |= (mem_read(REG(PC++)) << 8); if (F) { PUSH(REG(PC)); REG(PC) = n; cpu.currentCycleLength  += 3; }
#define RET(F)         if (F) { POP(REG(PC)); cpu.currentCycleLength += 4; }
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


#define PUSH(S)        mem_write(--REG(SP),(uint8_t) (S >> 8)); mem_write(--REG(SP), (uint8_t) (S & 0xFF)); 
#define POP(D)         D = mem_read(REG(SP)++); D |= mem_read(REG(SP)++) << 8;        



#define ENABLE_IRQ    cpu.irq_enable = 1;
#define DISABLE_IRQ   cpu.irq_enable = 0;
#define TRIGGER_IRQ(I)   if ( cpu.irq_enable && mem.IE & (1 << I) ) { mem.IF |= ( 1 << I); }


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
            union
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
    uint8_t currentCycleLength;
    uint8_t prefixCode;

    uint16_t _DIVhelper;
    uint16_t _TIMAhelper;

}cpu_t;







void cpu_init(void);
void cpu_tick(void);


void cpu_updateTimer();


















































































































































































#endif