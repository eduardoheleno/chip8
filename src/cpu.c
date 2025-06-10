#include "cpu.h"
#include "stack.h"

#include <string.h>

void init_cpu(cpu_t *c)
{
	memset(c->sys_ram, 0, sizeof(c->sys_ram));
	memset(c->sys_reg, 0, sizeof(c->sys_reg));

	memset(c->stack, 0, sizeof(c->stack));

	c->sp = 0;
	c->I = 0;
	c->pc = 0x200;
}

void jump_next_opcode(uint16_t *pc)
{
	*pc += 2;
}

void opcode_decoder(uint16_t opcode, cpu_t *cpu, video_t *video)
{
	switch (opcode & 0xF000) {
	case 0x0000:
		if ((opcode & 0x00FF) == 0xE0) {
			SDL_SetRenderDrawColor(video->renderer, 0x0, 0x0, 0x0, 0x0);
			SDL_RenderClear(video->renderer);
			SDL_RenderPresent(video->renderer);
		} else if ((opcode & 0x00FF) == 0xEE) {
			cpu->pc = stack_down(cpu->stack, &cpu->sp);
		}

		break;
	case 0x1000:
		cpu->pc = opcode & 0x0FFF;
		break;
	case 0x2000:
		stack_up(cpu->stack, &cpu->sp, cpu->pc);
		cpu->pc = opcode & 0x0FFF;
		break;
	case 0x3000: {
		uint8_t reg_idx = (opcode & 0x0F00) >> 8;
		uint8_t compare_value = (uint8_t)(opcode & 0x00FF);

		if (cpu->sys_reg[reg_idx] == compare_value) jump_next_opcode(&cpu->pc);

		break;
	}
	case 0x4000: {
		uint8_t reg_idx = (opcode & 0x0F00) >> 8;
		uint8_t compare_value = (uint8_t)(opcode & 0x00FF);

		if (cpu->sys_reg[reg_idx] != compare_value) jump_next_opcode(&cpu->pc);

		break;
	}
	case 0x5000: {
		uint8_t reg1_idx = (opcode & 0x0F00) >> 8;
		uint8_t reg2_idx = (opcode & 0x00F0) >> 4;

		if (cpu->sys_reg[reg1_idx] == cpu->sys_reg[reg2_idx]) jump_next_opcode(&cpu->pc);

		break;
	}
	case 0x6000: {
		uint8_t reg_idx = (opcode & 0x0F00) >> 8;
		uint8_t reg_set_value = (uint8_t)(opcode & 0x00FF);
		cpu->sys_reg[reg_idx] = reg_set_value;

		break;
	}
	case 0x7000: {
		uint8_t reg_idx = (opcode & 0x0F00) >> 8;
		uint8_t reg_add_value = (uint8_t)(opcode & 0x00FF);
		cpu->sys_reg[reg_idx] += reg_add_value;

		break;
	}
	case 0x8000: {
		uint8_t reg1_idx = (opcode & 0x0F00) >> 8;
		uint8_t reg2_idx = (opcode & 0x00F0) >> 4;

		switch (opcode & 0x000F) {
		case 0x0000:
			cpu->sys_reg[reg1_idx] = cpu->sys_reg[reg2_idx];
			break;
		case 0x0001:
			cpu->sys_reg[reg1_idx] = cpu->sys_reg[reg1_idx] | cpu->sys_reg[reg2_idx];
			break;
		case 0x0002:
			cpu->sys_reg[reg1_idx] = cpu->sys_reg[reg1_idx] & cpu->sys_reg[reg2_idx];
			break;
		case 0x0003:
			cpu->sys_reg[reg1_idx] = cpu->sys_reg[reg1_idx] ^ cpu->sys_reg[reg2_idx];
			break;
		case 0x0004: {
			uint16_t result = cpu->sys_reg[reg1_idx] + cpu->sys_reg[reg2_idx];
			if (result > 255) {
				cpu->sys_reg[0xF] = 1;
			} else {
				cpu->sys_reg[0xF] = 0;
			}

			cpu->sys_reg[reg1_idx] = (uint8_t)result;
			break;
		}
		case 0x0005:
			if (cpu->sys_reg[reg1_idx] > cpu->sys_reg[reg2_idx]) {
				cpu->sys_reg[0xF] = 1;
			} else {
				cpu->sys_reg[0xF] = 0;
			}

			cpu->sys_reg[reg1_idx] = cpu->sys_reg[reg1_idx] - cpu->sys_reg[reg2_idx];
			break;
		case 0x0006:
			if ((cpu->sys_reg[reg1_idx] & 1) == 1) {
				cpu->sys_reg[0xF] = 1;
			} else {
				cpu->sys_reg[0xF] = 0;
			}

			cpu->sys_reg[reg1_idx] >>= 1;
			break;
		case 0x0007:
			if (cpu->sys_reg[reg2_idx] > cpu->sys_reg[reg1_idx]) {
				cpu->sys_reg[0xF] = 1;
			} else {
				cpu->sys_reg[0xF] = 0;
			}

			cpu->sys_reg[reg1_idx] = cpu->sys_reg[reg2_idx] - cpu->sys_reg[reg1_idx];
			break;
		case 0x000E:
			// TODO
			break;
		}

		break;
	}
	case 0xA000:
		cpu->I = opcode & 0x0FFF;
		break;
	case 0xD000: {
		// TODO
		// - collision
		// - prevent render far beyond window size
		uint8_t sprite_height = (uint8_t)(opcode & 0x00F);
		uint16_t sprite_loc = cpu->I;
		uint8_t reg1_idx = (uint8_t)(opcode & 0x0F00);
		uint8_t reg2_idx = (uint8_t)(opcode & 0x00F0 >> 7);

		uint8_t target_x = cpu->sys_reg[reg1_idx];
		uint8_t target_y = cpu->sys_reg[reg2_idx];

		render_sprite(video->renderer,
					  cpu->sys_ram,
					  sprite_height,
					  sprite_loc,
					  target_x,
					  target_y);

		break;
	}
	}
}
