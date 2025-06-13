#include "cpu.h"
#include "stack.h"

#include <string.h>

void cpu_init(cpu_t *c)
{
	memset(c->sys_ram, 0, sizeof(c->sys_ram));
	memset(c->sys_reg, 0, sizeof(c->sys_reg));

	memset(c->stack, 0, sizeof(c->stack));

	c->sp = 0;
	c->I = 0;
	c->pc = PC_START_ADDRESS;

	c->delay_timer = 0;
}

void cpu_jump(uint16_t *pc)
{
	*pc += 2;
}

void cpu_decode(uint16_t opcode, cpu_t *cpu, video_t *video)
{
	switch (opcode & 0xF000) {
	case 0x0000:
		if ((opcode & 0x00FF) == 0xE0) {
			SDL_SetRenderDrawColor(video->renderer, 0x0, 0x0, 0x0, 0x0);
			SDL_RenderClear(video->renderer);
			SDL_RenderPresent(video->renderer);
		} else if ((opcode & 0x00FF) == 0xEE) {
			cpu->pc = stack_down(cpu->stack, &cpu->sp, opcode);
		}

		break;
	case 0x1000:
		cpu->pc = opcode & 0x0FFF;
		break;
	case 0x2000:
		stack_up(cpu->stack, &cpu->sp, cpu->pc, opcode);
		cpu->pc = opcode & 0x0FFF;
		break;
	case 0x3000: {
		uint8_t vx = (opcode & 0x0F00) >> 8;
		uint8_t compare_value = (uint8_t)(opcode & 0x00FF);

		if (cpu->sys_reg[vx] == compare_value) cpu_jump(&cpu->pc);

		break;
	}
	case 0x4000: {
		uint8_t vx = (opcode & 0x0F00) >> 8;
		uint8_t compare_value = (uint8_t)(opcode & 0x00FF);

		if (cpu->sys_reg[vx] != compare_value) cpu_jump(&cpu->pc);

		break;
	}
	case 0x5000: {
		uint8_t reg1_idx = (opcode & 0x0F00) >> 8;
		uint8_t reg2_idx = (opcode & 0x00F0) >> 4;

		if (cpu->sys_reg[reg1_idx] == cpu->sys_reg[reg2_idx]) cpu_jump(&cpu->pc);

		break;
	}
	case 0x6000: {
		uint8_t vx = (opcode & 0x0F00) >> 8;
		uint8_t reg_set_value = (uint8_t)(opcode & 0x00FF);
		cpu->sys_reg[vx] = reg_set_value;

		break;
	}
	case 0x7000: {
		uint8_t vx = (opcode & 0x0F00) >> 8;
		uint8_t reg_add_value = (uint8_t)(opcode & 0x00FF);
		cpu->sys_reg[vx] += reg_add_value;

		break;
	}
	case 0x8000: {
		uint8_t vx = (opcode & 0x0F00) >> 8;
		uint8_t vy = (opcode & 0x00F0) >> 4;

		switch (opcode & 0x000F) {
		case 0x0000:
			cpu->sys_reg[vx] = cpu->sys_reg[vy];
			break;
		case 0x0001:
			cpu->sys_reg[vx] = cpu->sys_reg[vx] | cpu->sys_reg[vy];
			break;
		case 0x0002:
			cpu->sys_reg[vx] = cpu->sys_reg[vx] & cpu->sys_reg[vy];
			break;
		case 0x0003:
			cpu->sys_reg[vx] = cpu->sys_reg[vx] ^ cpu->sys_reg[vy];
			break;
		case 0x0004: {
			uint16_t result = cpu->sys_reg[vx] + cpu->sys_reg[vy];
			if (result > 255) {
				cpu->sys_reg[0xF] = 1;
			} else {
				cpu->sys_reg[0xF] = 0;
			}

			cpu->sys_reg[vx] = (uint8_t)result;
			break;
		}
		case 0x0005:
			if (cpu->sys_reg[vx] > cpu->sys_reg[vy]) {
				cpu->sys_reg[0xF] = 1;
			} else {
				cpu->sys_reg[0xF] = 0;
			}

			cpu->sys_reg[vx] = cpu->sys_reg[vx] - cpu->sys_reg[vy];
			break;
		case 0x0006:
			cpu->sys_reg[0xF] = (cpu->sys_reg[vx] & 1);
			cpu->sys_reg[vx] >>= 1;
			break;
		case 0x0007:
			if (cpu->sys_reg[vy] > cpu->sys_reg[vx]) {
				cpu->sys_reg[0xF] = 1;
			} else {
				cpu->sys_reg[0xF] = 0;
			}

			cpu->sys_reg[vx] = cpu->sys_reg[vy] - cpu->sys_reg[vx];
			break;
		case 0x000E:
			cpu->sys_reg[0xF] = (cpu->sys_reg[vx] >> 7) & 1;
			cpu->sys_reg[vx] <<= 1;
			break;
		}

		break;
	}
	case 0x9000: {
		uint8_t vx = (opcode & 0x0F00) >> 8;
		uint8_t vy = (opcode & 0x00F0) >> 4;

		if (cpu->sys_reg[vx] != cpu->sys_reg[vy]) cpu_jump(&cpu->pc);
		break;
	}
	case 0xA000:
		cpu->I = opcode & 0x0FFF;
		break;
	case 0xB000:
		cpu->pc = (opcode & 0x0FFF) + cpu->sys_reg[0];
		break;
	case 0xC000: {
		uint8_t r = (uint8_t)rand();
		uint8_t vx = (opcode & 0x0F00) >> 8;
		uint8_t and_val = (uint8_t)(opcode & 0x00FF);

		cpu->sys_reg[vx] = r & and_val;
		break;
	}
	case 0xD000: {
		// TODO:
		// - collision
		// - prevent render far beyond window size
		uint8_t sprite_height = (uint8_t)(opcode & 0x00F);
		uint16_t sprite_loc = cpu->I;
		uint8_t vx = (uint8_t)(opcode & 0x0F00);
		uint8_t vy = (uint8_t)(opcode & 0x00F0 >> 7);

		uint8_t target_x = cpu->sys_reg[vx];
		uint8_t target_y = cpu->sys_reg[vy];

		video_render_sprite(video->renderer,
					  cpu->sys_ram,
					  sprite_height,
					  sprite_loc,
					  target_x,
					  target_y);

		break;
	}
	case 0xF000:
		switch (opcode & 0x00FF) {
		case 0x0029: {
			uint8_t vx = (opcode & 0x0F00) >> 8;
			uint8_t digit = cpu->sys_reg[vx];

			cpu->I = (uint16_t)(FONTSET_START_ADDRESS + (5 * digit));
			break;
		}
		}
		break;
	}
}
