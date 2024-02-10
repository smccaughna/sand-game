#pragma once

#include <SDL.h>

#include <stdbool.h>
#include <stdint.h>

enum
{
    NONE,
    SAND,
    WATER,
    ACID,
    VIRUS,
    WOOD,
    METAL
};

typedef struct material_t
{
    uint8_t type;
    SDL_Color color;
} material_t;

typedef struct particle_t
{
    material_t material;
    bool enabled;
    bool is_static;
    uint32_t update_step;
} particle_t;

extern void update_grid();
extern void clear_grid();
extern void activate_neighbors(int32_t, int32_t);

const extern material_t sand;
const extern material_t water;
const extern material_t acid;
const extern material_t virus;
const extern material_t wood;
const extern material_t metal;
const extern material_t none;
extern material_t current_material;

extern particle_t** cell_grid;

extern int32_t grid_width;
extern int32_t grid_height;
extern int32_t particle_count;
