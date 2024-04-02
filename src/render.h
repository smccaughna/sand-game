#pragma once

#include <stdbool.h>

typedef struct state state_t;

typedef struct button {
    SDL_Rect    rect;
    bool        selected;
} button_t;

extern void render(state_t* state);
extern void init_buttons(state_t* state);