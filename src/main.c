#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "SDL.h"

#include "cpu.h"
#include "video.h"
#include "stack.h"

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

int main(void)
{
	cpu_t cpu;
	video_t video;

	init_cpu(&cpu);
	init_video(&video);

	load_rom(cpu.sys_ram);

	bool quit = false;
	while (!quit) {
		SDL_Event e;
		SDL_PollEvent(&e);

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
			quit = true;
		}

		uint16_t opcode = (uint16_t)((cpu.sys_ram[cpu.pc] << 8) | cpu.sys_ram[cpu.pc + 1]);
		jump_next_opcode(&cpu.pc);

		opcode_decoder(opcode, &cpu, &video);
	}

	SDL_DestroyRenderer(video.renderer);
	SDL_DestroyWindow(video.window);

	SDL_Quit();

	return 0;
}
