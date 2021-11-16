#include "gb_t.h"

#define DEBUG

// 5 interrupts




cpu_t * init_cpu();

uint8_t load_rom(cpu_t *cpu, char* path);

void tick_cpu(cpu_t *cpu);

void fake_powerup(cpu_t *);


