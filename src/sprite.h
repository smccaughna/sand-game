#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct state state_t;

extern bool load_media(state_t* state);
extern void unload_media(state_t* state);

extern const SDL_Color white;
extern const SDL_Color black;