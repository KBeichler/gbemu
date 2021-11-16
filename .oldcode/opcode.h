
#include "gb_t.h"






// ACCESS FUNCTIONS
#define REG(r) cpu->reg.r
#define MEM(mr) cpu->mem.data[mr]
#define FLAG(f) cpu->reg.flags.f


#define ARG_1(D)  (( D & 0x38 ) >> 3) 
#define ARG_2(D)  (( D & 0x07 )     )

// LD_R_R
// D Destination, S source
#define LD_R_n(D, S)  D = S
#define LD_R_nn(D) { D = (MEM(REG(PC++)) | (MEM(REG(PC++))  << 8) );  }




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

#define ADD_SP        { FLAG(Z) = FLAG(N) = 0; uint8_t i = MEM(REG(PC++)); REG(SP += (int8_t) i;)}
// 16 BIT ARITHEMITC
// S = Register
#define INC_RR(S)     { S++;  }
#define DEC_RR(S)     { S--;  }
#define ADD_RR(S)     { FLAG(N) = 1; FLAG(HC) = ((S % 0x0FFF) + (REG(HL) & 0x0FFF)) > 0x0FFF; REG(HL) += S; FLAG(CY) == (REG(HL) - S) < 0x0;   }

// JMP AND CALL 
// F = Condition to execute
#define JMP_n(F)      {  uint8_t i = MEM(REG(PC)++); if (F) {REG(PC) += (int8_t) i; cpu->cycle += 3;} }
#define JMP_nn(F)     { uint16_t i = MEM(REG(PC++)); i |= (MEM(REG(PC++)) << 8); if (F) {REG(PC) = i; cpu->cycle += 1;} ;}
#define CALL(F)       { uint16_t i = MEM(REG(PC++)); i |= (MEM(REG(PC++)) << 8);  PUSH(REG(PC)); if (F) {REG(PC) = i; cpu->cycle += 3;} ; }
#define RET(F)        { if (F) { POP(REG(PC)); cpu->cycle += 4; }; }

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


#define PUSH(S)       { MEM(--REG(SP)) = (S >> 8); MEM(--REG(SP))= S & 0xFF ; } 
#define PUSHA         { MEM(--REG(SP)) = REG(A)  ; MEM(--REG(SP))= FLAG(R); }
#define POP(D)        { D = MEM(REG(SP++)); D |= MEM(REG(SP++))  << 8;        }
#define POPA          { FLAG(R) = MEM(REG(SP++)); REG(A) = MEM(REG(SP++));  }
#define RST(I)        { PUSH( (REG(PC)-1) );  REG(PC) = 0x0000 + I;           }

#define ENABLE_IRQ    cpu->irq_enable = 1;
#define DISABLE_IRQ   cpu->irq_enable = 0;







void run_Op(cpu_t *cpu);

void run_PrefixOp(cpu_t *cpu);

uint8_t fetch_Op(cpu_t *cpu);



