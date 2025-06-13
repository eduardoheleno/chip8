#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include "video.h"

#define FONTSET_START_ADDRESS 0x50
#define DEFAULT_FONTS_SIZE 80

#define PC_START_ADDRESS 0x200

typedef struct cpu {
	uint8_t sys_ram[4096];
	uint8_t sys_reg[16];

	uint16_t stack[16];
	uint8_t sp;

	uint16_t I;
	uint16_t pc;

	uint8_t delay_timer;
} cpu_t;

void cpu_init(cpu_t *c);
void cpu_decode(uint16_t opcode, cpu_t *cpu, video_t *video);
void cpu_jump(uint16_t *pc);

#endif // CPU_H_
