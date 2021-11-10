#ifndef GB
#define GB

#include <stdio.h>
#include <stdint.h>
#include <cpu.h>
#include <mem.h>
#include <ppu.h>






// REGISTER DEFINITIONS
// PORT MODE REGISTER
#define P1      0xFF00
#define SB      0xFF01
#define SC      0xFF02
#define DIV     0xFF04
#define TIMA    0xFF05
#define TMA     0xFF06
#define TAC     0xFF07

// IRQ REGISTERS
#define I_FLAG   0xFF0F
#define I_ENABLE 0xFFFF
// IRQ MASKS
#define IRQ_VBLANK  0
#define IRQ_LCDC    1
#define IRQ_TIMER   2
#define IRQ_SERIAL  3
#define IRQ_IO      4
// IRQ MEM SPACES
// VBLANK 0x0040 LCDC 0x0048 TImoer 0x0050  SErail 0x0058  IO 0x0060 //falling priority

// LCD REGISTER 
#define LCDC    0xFF40
#define STAT    0xFF41
#define SCY     0xFF42
#define SCX     0xFF43
#define LY      0xFF44
#define LYC     0xFF45
#define DMA     0xFF46  
#define BGP     0xFF47
#define OBP0    0xFF48
#define OPB1    0xFF49
#define WY      0xFF4A
#define WX      0xFF4B





























#endif