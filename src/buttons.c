#include <SDL2/SDL.h>
#include <getopt.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buttons.h"

uint16_t button_bitmap = 0;

static const uint16_t key_map[SDL_NUM_SCANCODES] = {
    [SDL_SCANCODE_S]      = 0b0000000001, // A
    [SDL_SCANCODE_A]      = 0b0000000010, // B
    [SDL_SCANCODE_W]      = 0b0000000100, // X
    [SDL_SCANCODE_Q]      = 0b0000001000, // Y
    [SDL_SCANCODE_LSHIFT] = 0b0000010000, // L shoulder
    [SDL_SCANCODE_RSHIFT] = 0b0000100000, // R shoulder
    [SDL_SCANCODE_UP]     = 0b0001000000, // d-pad up
    [SDL_SCANCODE_DOWN]   = 0b0010000000, // d-pad down
    [SDL_SCANCODE_LEFT]   = 0b0100000000, // d-pad left
    [SDL_SCANCODE_RIGHT]  = 0b1000000000, // d-pad right
};

uint16_t key_convert(int sdlcode) {
    if (sdlcode < 0 || sdlcode > SDL_NUM_SCANCODES) return 0;
    return key_map[sdlcode];
}

void key_pressed(int sdlcode) {
    uint16_t code = key_convert(sdlcode);
    if (code) button_bitmap |= code;
}

void key_released(int sdlcode) {
    uint16_t code = key_convert(sdlcode) | 0x80;
    if (code) button_bitmap &= ~(code);
}
