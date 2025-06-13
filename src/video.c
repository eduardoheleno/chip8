#include "video.h"
#include "stdbool.h"

void video_init(video_t *video)
{
	SDL_Init(SDL_INIT_VIDEO);
	video->window = SDL_CreateWindow("ch8",
						SDL_WINDOWPOS_UNDEFINED,
						SDL_WINDOWPOS_UNDEFINED,
						SCREEN_W,
						SCREEN_H,
						SDL_WINDOW_SHOWN);
	if (video->window == NULL) {
		perror("Error while trying to create SDL window");
		exit(EXIT_FAILURE);
	}

	video->renderer = SDL_CreateRenderer(video->window, -1, SDL_RENDERER_ACCELERATED);
	if (video->renderer == NULL) {
		perror("Error while trying to init renderer instance");
		exit(EXIT_FAILURE);
	}
}

void video_render_sprite(SDL_Renderer *r,
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
