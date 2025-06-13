#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdbool.h>

#include "SDL_keycode.h"
#include "SDL_events.h"

#define KEYBOARD_BUFFER_SIZE 16

typedef enum {
	KEY_1 = 0,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_UNKNOWN = -1
} key;

key keyboard_map_sdl_key(SDL_Keycode keycode);
void keyboard_decode(SDL_Event e, bool *quit, int *keyboard_buffer);

#endif // KEYBOARD_H_
