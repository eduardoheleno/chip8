#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#include "SDL.h"

#include "cpu.h"
#include "video.h"
#include "stack.h"
#include "keyboard.h"

void load_default_font(uint8_t *sys_ram)
{
	uint8_t fontset[DEFAULT_FONTS_SIZE] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	memcpy(&sys_ram[FONTSET_START_ADDRESS], fontset, 80);
}

void load_rom(uint8_t *sys_ram)
{
	FILE *fp = fopen("ibmlogo.ch8", "rb");

	if (fp == NULL) {
		perror("Error while loading the rom.\n");
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0, SEEK_END);
	size_t file_size = (size_t)ftell(fp);
	rewind(fp);

	if (file_size <= 0 || file_size > (4096 - 0x200)) {
		fprintf(stderr, "ROM size is invalid or too large: %ld bytes\n", file_size);
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	size_t read_bytes = fread(&sys_ram[0x200], 1, file_size, fp);
	if (read_bytes != file_size) {
		fprintf(stderr, "Failed to read the ROM file.\n");
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	fclose(fp);
}

uint64_t current_time_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int main(void)
{
	uint64_t last_timer_tick = current_time_ms();

	cpu_t cpu;
	video_t video;
	int keyboard_buffer[KEYBOARD_BUFFER_SIZE] = {0};

	srand((unsigned int)time(NULL));

	cpu_init(&cpu);
	video_init(&video);

	load_default_font(cpu.sys_ram);
	load_rom(cpu.sys_ram);

	bool quit = false;
	while (!quit) {
		SDL_Event e;
		SDL_PollEvent(&e);

		keyboard_decode(e, &quit, keyboard_buffer);

		uint16_t opcode = (uint16_t)((cpu.sys_ram[cpu.pc] << 8) | cpu.sys_ram[cpu.pc + 1]);
		cpu_jump(&cpu.pc);

		cpu_decode(opcode, &cpu, &video);

		uint64_t now = current_time_ms();
		if (now - last_timer_tick >= 1000 / 60) {
			if (cpu.delay_timer > 0) cpu.delay_timer--;

			last_timer_tick = now;
		}
	}

	SDL_DestroyRenderer(video.renderer);
	SDL_DestroyWindow(video.window);

	SDL_Quit();

	return 0;
}
