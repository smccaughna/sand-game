#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

extern int32_t load_media();
extern void unload_media();
extern int32_t create_text(const char*, TTF_Font*, SDL_Color, SDL_Texture**);

extern SDL_Texture* tx_icon_sand;
extern SDL_Texture* tx_icon_water;
extern SDL_Texture* tx_icon_acid;
extern SDL_Texture* tx_icon_virus;
extern SDL_Texture* tx_icon_wood;
extern SDL_Texture* tx_icon_metal;
extern SDL_Texture* tx_icon_eraser;
extern SDL_Texture* tx_icon_reset;

extern SDL_Texture* tx_icon_sand_selected;
extern SDL_Texture* tx_icon_water_selected;
extern SDL_Texture* tx_icon_acid_selected;
extern SDL_Texture* tx_icon_virus_selected;
extern SDL_Texture* tx_icon_wood_selected;
extern SDL_Texture* tx_icon_metal_selected;
extern SDL_Texture* tx_icon_eraser_selected;
extern SDL_Texture* tx_icon_reset_selected;

extern TTF_Font* ttf_cascadia_code;

extern SDL_Texture* tx_1;
extern SDL_Texture* tx_2;
extern SDL_Texture* tx_3;
extern SDL_Texture* tx_4;
extern SDL_Texture* tx_5;
extern SDL_Texture* tx_6;
extern SDL_Texture* tx_0;
extern SDL_Texture* tx_c;

extern SDL_Texture* tx_fps_counter;
extern SDL_Texture* tx_selected_material;
extern SDL_Texture* tx_advanced_options;
extern SDL_Texture* tx_particle_count;