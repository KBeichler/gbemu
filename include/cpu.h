#ifndef CPU
#define CPU

#include <gb.h>


// ACCESS FUNCTIONS
#define REG(r) cpu.r
//#define READMEM(adr) mem_read(adr);
//#define WRITEMEM(adr, val) mem_write(mr, val;
#define FLAG(f) cpu.f


#define ARG_1(D)  (( D & 0x38 ) >> 3) 
#define ARG_2(D)  (( D & 0x07 )     )

// LD_R_R
// D Destination, S source
#define LD_R_n(D, S)  D = S
#define LD_R_nn(D) { D |=  mem_read(REG(PC++)); D |= mem_read(REG(PC++))  << 8 ;  }

#define LD_MEM_n(D, S)  mem_write(D, S);




// 8 BIT ARITHMETIC
// S = Register
#define ADD_R(S)      { uint8_t I = S; FLAG(HC) =  ((REG(A) & 0xF) + (I & 0xF)) > 0xF;  REG(A) += I; FLAG(Z) = (REG(A) == 0); FLAG(N) = 0; FLAG(CY) = (REG(A) - I) < 0x00; }
#define SUB_R(S)      { uint8_t I = S; FLAG(HC) =   (REG(A) & 0xF) < (I & 0xF) ;        REG(A) -= I; FLAG(Z) = (REG(A) == 0); FLAG(N) = 1; FLAG(CY) = (REG(A) + I) > 0xFF; }
#define ADC_R(S)      { uint8_t i = S + FLAG(CY); ADD_R( i );  }
#define SBC_R(S)      { uint8_t i = S + FLAG(CY); SUB_R( i );  }
#define AND_R(S)      { FLAG(N) = FLAG(CY) = 0; FLAG(HC) = 1; REG(A) &= S; FLAG(Z) = (REG(A) == 0);  }
#define XOR_R(S)      { FLAG(N) = FLAG(CY) =    FLAG(HC) = 0; REG(A) ^= S; FLAG(Z) = (REG(A) == 0);  }
#define OR_R(S)       { FLAG(N) = FLAG(CY) =    FLAG(HC) = 0; REG(A) |= S; FLAG(Z) = (REG(A) == 0);  }
#define INC_R(S)      { FLAG(N) = 0; FLAG(HC) = (S & 0xF) == 0xF; S++; FLAG(Z) = S == 0; }
#define DEC_R(S)      { FLAG(N) = 1; FLAG(HC) = (S ==  0);        S--; FLAG(Z) = S == 0; }

#define CP_R(S)       { uint8_t i = S; FLAG(N) = 1; FLAG(Z) = ( REG(A) == i ); FLAG(CY) = REG(A) < i; FLAG(HC) = ((REG(A) & 0xF) < (i & 0xF)); }

#define ADD_SP        { FLAG(Z) = FLAG(N) = 0; uint8_t i = mem_read(REG(PC)++); REG(SP += (int8_t) i;)}
// 16 BIT ARITHEMITC
// S = Register
#define INC_RR(S)     { S++;  }
#define DEC_RR(S)     { S--;  }
#define ADD_RR(S)     { FLAG(N) = 1; FLAG(HC) = ((S % 0x0FFF) + (REG(HL) & 0x0FFF)) > 0x0FFF; REG(HL) += S; FLAG(CY) = (REG(HL) - S) < 0x0;   }

// JMP AND CALL 
// F = Condition to execute
#define JMP_n(F)      {  uint8_t i = mem_read(REG(PC)++); if (F) {REG(PC) += (int8_t) i; cpu.clock  += 3;} }
#define JMP_nn(F)     { uint16_t i = mem_read(REG(PC)++); i |= (mem_read(REG(PC)++) << 8); if (F) {REG(PC) = i; cpu.clock  += 1;} ;}
#define CALL(F)       { uint16_t i = mem_read(REG(PC)++); i |= (mem_read(REG(PC)++) << 8);  PUSH(REG(PC)); if (F) {REG(PC) = i; cpu.clock  += 3;} ; }
#define RET(F)        { if (F) { POP(REG(PC)); cpu.clock += 4; }; }

// SHIFT OPERATIONS
#define RLC(S)         { FLAG(N) = FLAG(HC) = 0; FLAG(CY) = !!(S & 0x80) ; S = (S << 1) + (FLAG(CY) > 0) ; FLAG(Z) = S == 0;	}
#define RL(S)          { FLAG(N) = FLAG(HC) = 0; short f  = S >> 7       ; S = (S << 1) | FLAG(CY)       ; FLAG(Z) = S == 0;	FLAG(CY) = !!f;  }
#define RRC(S)         { FLAG(N) = FLAG(HC) = 0; FLAG(CY) = S & 0x01     ; S = (S >> 1) | (FLAG(CY) << 7); FLAG(Z) = S == 0;	}
#define RR(S)          { FLAG(N) = FLAG(HC) = 0; short f  = S & 0x01     ; S = (S >> 1) | (FLAG(CY) << 7); FLAG(Z) = S == 0;  FLAG(CY) = !!f;	 }
#define SLA(S)         { FLAG(N) = FLAG(HC) = 0; FLAG(CY) = !!(S  & 0x80); S = S << 1                    ; FLAG(Z) = S == 0;	}
#define SRA(S)         { FLAG(N) = FLAG(HC) = 0; FLAG(CY) = S & 0x01     ; S = (S & 0x80) | (S >> 1)     ; FLAG(Z) = S == 0;	}
#define SRL(S)         { FLAG(N) = FLAG(HC) = 0; FLAG(CY) = S & 0x01     ; S = (S >> 1)                  ; FLAG(Z) = S == 0;	}
#define SWAP(S)        { FLAG(N) = FLAG(CY) =    FLAG(HC) = 0            ; S = S >> 4 | S << 4           ; FLAG(Z) = S == 0;  }
#define RES(B , S)     { S &= ~( 1 << B );  }
#define SET(B , S)     { S |=  ( 1 << B );  }
#define BIT(B , S)     { FLAG(N) = 0; FLAG(HC) = 1; FLAG(Z) = !( ( S & (1 << B)) );  }


//#define PUSH(S)       { MEM(--REG(SP)) = (S >> 8); MEM(--REG(SP))= S & 0xFF ; } 
#define PUSH(S)       { mem_write(--REG(SP),(uint8_t) (S >> 8)); mem_write(--REG(SP), (uint8_t) (S & 0xFF)); } 
//#define PUSHA         { MEM(--REG(SP)) = REG(A)  ; MEM(--REG(SP))= FLAG(R); }
//#define PUSHA         { write_mem(--REG(SP), REG(A) ; MEM(--REG(SP))= FLAG(R); }
#define POP(D)        { D = mem_read(REG(SP)++); D |= mem_read(REG(SP)++)  << 8;        }

#define RST(I)        { PUSH( (REG(PC)-1) );  REG(PC) = 0x0000 + I;           }

#define ENABLE_IRQ    cpu.irq_enable = 1;
#define DISABLE_IRQ   cpu.irq_enable = 0;



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