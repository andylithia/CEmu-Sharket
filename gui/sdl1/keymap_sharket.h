#ifndef KEYMAP_SHARKET_H
#define KEYMAP_SHARKET_H

#include <SDL.h>

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    SDLKey key;
    int8_t row, col;
    const char* keyname;
} cemu_sdl_key_t;

extern const cemu_sdl_key_t sharket_keymap[];

#endif