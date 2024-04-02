#pragma once

#include <SDL.h>
#include <stdint.h>

enum materials {
    SAND,
    WATER,
    ACID,
    VIRUS,
    WOOD,
    METAL,
    ERASER,
    RESET
};

typedef struct state state_t;

typedef struct particle {
    struct {
        int8_t type;
        SDL_Color color;
    } material;
    bool is_static;
    uint64_t last_update;
} particle_t;

extern void init_grid(state_t* state);
extern void update_grid(state_t* state);
extern void clear_grid(state_t* state);
extern void update_grid_size(state_t* state);

extern void activate_neighbors(state_t* state, int32_t x, int32_t y);

extern const SDL_Color color_sand;
extern const SDL_Color color_water;
extern const SDL_Color color_acid;
extern const SDL_Color color_virus;
extern const SDL_Color color_wood;
extern const SDL_Color color_metal;