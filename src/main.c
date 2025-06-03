#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "SDL.h"

#include "cpu.h"
#include "stack.h"

#define SCREEN_W 1280
#define SCREEN_H 640

void load_rom(uint8_t *sys_ram)
{
	FILE *fp = fopen("ibmlogo.ch8", "rb");

	if (fp == NULL) {
		perror("Error while loading the rom.\n");
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0, SEEK_END);
	size_t length = (size_t)ftell(fp);
	rewind(fp);

	char rom[length];
	fgets(rom, (int)length + 1, fp);

	fclose(fp);

	memcpy(&sys_ram[0x200], rom, length);
}

void init_video(SDL_Window **w, SDL_Renderer **r)
{
	SDL_Init(SDL_INIT_VIDEO);
	*w = SDL_CreateWindow("ch8",
						SDL_WINDOWPOS_UNDEFINED,
						SDL_WINDOWPOS_UNDEFINED,
						SCREEN_W,
						SCREEN_H,
						SDL_WINDOW_SHOWN);
	if (*w == NULL) {
		perror("Error while trying to create SDL window");
		exit(EXIT_FAILURE);
	}

	*r = SDL_CreateRenderer(*w, -1, SDL_RENDERER_ACCELERATED);
	if (*r == NULL) {
		perror("Error while trying to init renderer instance");
		exit(EXIT_FAILURE);
	}
}

void render_sprite(SDL_Renderer *r,
				   uint8_t *sys_ram,
				   uint8_t sprite_height,
				   uint16_t sprite_loc,
				   uint8_t target_x,
				   uint8_t target_y)
{
	uint16_t x = (uint16_t)target_x * 20;
	uint16_t y = (uint16_t)target_y * 20;

	for (int i = 0; i < sprite_height; i++) {
		uint8_t sprite_row = sys_ram[sprite_loc + i];
		for (int j = 0; j < 8; j++) {
			bool bit = (sprite_row >> (7 - j)) & 1;
			if (bit) {
				SDL_Rect pixel = {x, y, 20, 20};

				SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderFillRect(r, &pixel);

			}

			x += 20;
		}

		x = target_x * 20;
		y += 20;
	}

	SDL_RenderPresent(r);
}

void jump_next_opcode(uint16_t *pc)
{
	*pc += 2;
}

int main(void)
{
	cpu_t cpu;
	init_cpu(&cpu);

	uint16_t stack[16] = {0};
	uint8_t sp = 0;

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	load_rom(cpu.sys_ram);

	init_video(&window, &renderer);

	bool quit = false;
	while (!quit) {
		SDL_Event e;
		SDL_PollEvent(&e);

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
			quit = true;
		}

		uint16_t opcode = (uint16_t)((cpu.sys_ram[cpu.pc] << 8) | cpu.sys_ram[cpu.pc + 1]);
		jump_next_opcode(&cpu.pc);

		switch (opcode & 0xF000) {
		case 0x0000:
			if ((opcode & 0x00FF) == 0xE0) {
				SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);
				SDL_RenderClear(renderer);
				SDL_RenderPresent(renderer);
			} else if ((opcode & 0x00FF) == 0xEE) {
				cpu.pc = stack_down(stack, &sp);
			}

			break;
		case 0x1000:
			cpu.pc = opcode & 0x0FFF;
			break;
		case 0x2000:
			stack_up(stack, &sp, cpu.pc);
			cpu.pc = opcode & 0x0FFF;
			break;
		case 0x3000: {
			uint8_t reg_idx = (opcode & 0x0F00) >> 8;
			uint8_t compare_value = (uint8_t)(opcode & 0x00FF);

			if (cpu.sys_reg[reg_idx] == compare_value) jump_next_opcode(&cpu.pc);

			break;
		}
		case 0x4000: {
			uint8_t reg_idx = (opcode & 0x0F00) >> 8;
			uint8_t compare_value = (uint8_t)(opcode & 0x00FF);

			if (cpu.sys_reg[reg_idx] != compare_value) jump_next_opcode(&cpu.pc);

			break;
		}
		case 0x5000: {
			uint8_t reg1_idx = (opcode & 0x0F00) >> 8;
			uint8_t reg2_idx = (opcode & 0x00F0) >> 4;

			if (cpu.sys_reg[reg1_idx] == cpu.sys_reg[reg2_idx]) jump_next_opcode(&cpu.pc);

			break;
		}
		case 0x6000: {
			uint8_t reg_idx = (opcode & 0x0F00) >> 8;
			uint8_t reg_set_value = (uint8_t)(opcode & 0x00FF);
			cpu.sys_reg[reg_idx] = reg_set_value;

			break;
		}
		case 0x7000: {
			uint8_t reg_idx = (opcode & 0x0F00) >> 8;
			uint8_t reg_add_value = (uint8_t)(opcode & 0x00FF);
			cpu.sys_reg[reg_idx] += reg_add_value;

			break;
		}
		case 0x8000: {
			uint8_t reg1_idx = (opcode & 0x0F00) >> 8;
			uint8_t reg2_idx = (opcode & 0x00F0) >> 4;

			switch (opcode & 0x000F) {
			case 0x0000:
				cpu.sys_reg[reg1_idx] = cpu.sys_reg[reg2_idx];
				break;
			case 0x0001:
				cpu.sys_reg[reg1_idx] = cpu.sys_reg[reg1_idx] | cpu.sys_reg[reg2_idx];
				break;
			case 0x0002:
				cpu.sys_reg[reg1_idx] = cpu.sys_reg[reg1_idx] & cpu.sys_reg[reg2_idx];
				break;
			case 0x0003:
				cpu.sys_reg[reg1_idx] = cpu.sys_reg[reg1_idx] ^ cpu.sys_reg[reg2_idx];
				break;
			case 0x0004: {
				uint16_t result = cpu.sys_reg[reg1_idx] + cpu.sys_reg[reg2_idx];
				if (result > 255) {
					cpu.sys_reg[0xF] = 1;
				} else {
					cpu.sys_reg[0xF] = 0;
				}

				cpu.sys_reg[reg1_idx] = (uint8_t)result;
				break;
			}
			case 0x0005:
				if (cpu.sys_reg[reg1_idx] > cpu.sys_reg[reg2_idx]) {
					cpu.sys_reg[0xF] = 1;
				} else {
					cpu.sys_reg[0xF] = 0;
				}

				cpu.sys_reg[reg1_idx] = cpu.sys_reg[reg1_idx] - cpu.sys_reg[reg2_idx];
				break;
			}

			break;
		}
		case 0xA000:
			cpu.I = opcode & 0x0FFF;
			break;
		case 0xD000: {
			uint8_t sprite_height = (uint8_t)(opcode & 0x00F);
			uint16_t sprite_loc = cpu.I;
			uint8_t reg1_idx = (uint8_t)(opcode & 0x0F00);
			uint8_t reg2_idx = (uint8_t)(opcode & 0x00F0 >> 7);

			uint8_t target_x = cpu.sys_reg[reg1_idx];
			uint8_t target_y = cpu.sys_reg[reg2_idx];

			render_sprite(renderer,
						  cpu.sys_ram,
						  sprite_height,
						  sprite_loc,
						  target_x,
						  target_y);

			break;
		}
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
