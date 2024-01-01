#pragma once

#include <SDL.h>

#define SCALE 2

typedef struct button_t
{
    int selected;
    SDL_Rect rect;
    SDL_Texture* texture;
    SDL_Texture* selected_texture;
} button_t;

extern void draw_screen();
extern void update_grid_size();

extern int32_t window_width;
extern int32_t window_height;

extern button_t button_sand;
extern button_t button_water;
extern button_t button_acid;
extern button_t button_virus;
extern button_t button_wood;
extern button_t button_metal;
extern button_t button_eraser;
extern button_t button_reset;

extern char str_fps_counter[10];
extern char str_particle_count[21];
extern int32_t length_selected_material;

extern int32_t show_advanced_options;
extern int32_t show_static_particles;
extern int32_t show_fps;
extern int32_t show_particle_count;