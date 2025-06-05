#ifndef VIDEO_H_
#define VIDEO_H_

#include "SDL.h"

#define SCREEN_W 1280
#define SCREEN_H 640

typedef struct video {
	SDL_Window *window;
	SDL_Renderer *renderer;
} video_t;

void init_video(video_t *video);
void render_sprite(SDL_Renderer *r,
				   uint8_t *sys_ram,
				   uint8_t sprite_height,
				   uint16_t sprite_loc,
				   uint8_t target_x,
				   uint8_t target_y);

#endif // VIDEO_H_
