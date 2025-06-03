#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

typedef struct cpu {
	uint8_t sys_ram[4096];
	uint8_t sys_reg[16];

	uint16_t I;
	uint16_t pc;
} cpu_t;

 void init_cpu(cpu_t *c);

#endif // CPU_H_
