#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include "video.h"

typedef struct cpu {
	uint8_t sys_ram[4096];
	uint8_t sys_reg[16];

	uint16_t stack[16];
	uint8_t sp;

	uint16_t I;
	uint16_t pc;
} cpu_t;

void init_cpu(cpu_t *c);
void opcode_decoder(uint16_t opcode, cpu_t *cpu, video_t *video);
void jump_next_opcode(uint16_t *pc);

#endif // CPU_H_
