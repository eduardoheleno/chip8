#include "keyboard.h"
#include "SDL_events.h"

key keyboard_map_sdl_key(SDL_Keycode keycode)
{
	switch (keycode) {
		case SDLK_1:
			return KEY_1;
		case SDLK_2:
			return KEY_2;
		case SDLK_3:
			return KEY_3;
		case SDLK_4:
			return KEY_4;
		case SDLK_q:
			return KEY_Q;
		case SDLK_w:
			return KEY_W;
		case SDLK_e:
			return KEY_E;
		case SDLK_r:
			return KEY_R;
		case SDLK_a:
			return KEY_A;
		case SDLK_s:
			return KEY_S;
		case SDLK_d:
			return KEY_D;
		case SDLK_f:
			return KEY_F;
		case SDLK_z:
			return KEY_Z;
		case SDLK_x:
			return KEY_X;
		case SDLK_c:
			return KEY_C;
		case SDLK_v:
			return KEY_V;
		default:
			return KEY_UNKNOWN;
	}
}

void keyboard_decode(SDL_Event e, bool *quit, int *keyboard_buffer)
{
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_ESCAPE) {
			*quit = true;
		} else {
			key k = keyboard_map_sdl_key(e.key.keysym.sym);
			keyboard_buffer[k] = 1;
		}
	} else if (e.type == SDL_KEYUP) {
		printf("keyup\n");
		key k = keyboard_map_sdl_key(e.key.keysym.sym);
		keyboard_buffer[k] = 0;
	}
}
