#include "global.h"
#include "sprite.h"
#include "screen.h"

int32_t load_media();
void unload_media();
int32_t create_text(const char*, TTF_Font*, SDL_Color, SDL_Texture**);

static int32_t load_texture(const char*, SDL_Texture**);
static int32_t load_font(const char*, TTF_Font**, int32_t);

SDL_Texture* tx_icon_sand;
SDL_Texture* tx_icon_water;
SDL_Texture* tx_icon_acid;
SDL_Texture* tx_icon_virus;
SDL_Texture* tx_icon_wood;
SDL_Texture* tx_icon_metal;
SDL_Texture* tx_icon_eraser;
SDL_Texture* tx_icon_reset;

SDL_Texture* tx_icon_sand_selected;
SDL_Texture* tx_icon_water_selected;
SDL_Texture* tx_icon_acid_selected;
SDL_Texture* tx_icon_virus_selected;
SDL_Texture* tx_icon_wood_selected;
SDL_Texture* tx_icon_metal_selected;
SDL_Texture* tx_icon_eraser_selected;
SDL_Texture* tx_icon_reset_selected;

TTF_Font* ttf_cascadia_code;

SDL_Texture* tx_1;
SDL_Texture* tx_2;
SDL_Texture* tx_3;
SDL_Texture* tx_4;
SDL_Texture* tx_5;
SDL_Texture* tx_6;
SDL_Texture* tx_0;
SDL_Texture* tx_c;

SDL_Texture* tx_fps_counter;
SDL_Texture* tx_selected_material;
SDL_Texture* tx_advanced_options;
SDL_Texture* tx_particle_count;

char* str_advanced_options = "Advanced options:\n"
                             "\n"
                             "[F1]: toggle this menu\n"
                             "[F2]: toggle framerate limit\n"
                             "[F3]: pause/unpause simulation\n"
                             "[F4]: show static particles\n"
                             "[F5]: show FPS\n"
                             "[F6]: show particle count";

static int32_t load_texture(const char* file, SDL_Texture** tex)
{
    *tex = IMG_LoadTexture(renderer, file);
    if (*tex == NULL)
    {
        printf("Error loading texture %s: %s\n", file, IMG_GetError());
        return 0;
    }

    return 1;
}

static int32_t load_font(const char* file, TTF_Font** font, int32_t size)
{
    *font = TTF_OpenFont(file, size);
    if (*font == NULL)
    {
        printf("Error loading font %s: %s\n", file, TTF_GetError());
        return 0;
    }

    return 1;
}

int32_t create_text(const char* text, TTF_Font* font, SDL_Color color, SDL_Texture** tex)
{
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text, color, window_width);
    if (surface == NULL)
    {
        printf("Failed to create surface: %s\n", TTF_GetError());
        return 0;
    }

    *tex = SDL_CreateTextureFromSurface(renderer, surface);
    if (*tex == NULL)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
        return 0; 
    }

    SDL_FreeSurface(surface);
    surface = NULL;

    return 1;
}

int32_t load_media()
{
    return
        load_texture("res/sprites/icon_sand.png", &tx_icon_sand) && 
        load_texture("res/sprites/icon_water.png", &tx_icon_water) &&
        load_texture("res/sprites/icon_acid.png", &tx_icon_acid) && 
        load_texture("res/sprites/icon_virus.png", &tx_icon_virus) && 
        load_texture("res/sprites/icon_wood.png", &tx_icon_wood) && 
        load_texture("res/sprites/icon_metal.png", &tx_icon_metal) && 
        load_texture("res/sprites/icon_eraser.png", &tx_icon_eraser) && 
        load_texture("res/sprites/icon_reset.png", &tx_icon_reset) && 
        load_texture("res/sprites/icon_sand_selected.png", &tx_icon_sand_selected) && 
        load_texture("res/sprites/icon_water_selected.png", &tx_icon_water_selected) &&
        load_texture("res/sprites/icon_acid_selected.png", &tx_icon_acid_selected) && 
        load_texture("res/sprites/icon_virus_selected.png", &tx_icon_virus_selected) &&
        load_texture("res/sprites/icon_wood_selected.png", &tx_icon_wood_selected) && 
        load_texture("res/sprites/icon_metal_selected.png", &tx_icon_metal_selected) && 
        load_texture("res/sprites/icon_eraser_selected.png", &tx_icon_eraser_selected) && 
        load_texture("res/sprites/icon_reset_selected.png", &tx_icon_reset_selected) && 
        load_font("res/fonts/CascadiaCode.ttf", &ttf_cascadia_code, 72) && 
        create_text("1", ttf_cascadia_code, white, &tx_1) && 
        create_text("2", ttf_cascadia_code, white, &tx_2) && 
        create_text("3", ttf_cascadia_code, white, &tx_3) && 
        create_text("4", ttf_cascadia_code, white, &tx_4) && 
        create_text("5", ttf_cascadia_code, white, &tx_5) && 
        create_text("6", ttf_cascadia_code, white, &tx_6) && 
        create_text("0", ttf_cascadia_code, white, &tx_0) && 
        create_text("c", ttf_cascadia_code, white, &tx_c) && 
        create_text("sand", ttf_cascadia_code, white, &tx_selected_material) && 
        create_text(str_advanced_options, ttf_cascadia_code, white, &tx_advanced_options);
}

void unload_media()
{
    SDL_DestroyTexture(tx_icon_sand); tx_icon_sand = NULL;
    SDL_DestroyTexture(tx_icon_water); tx_icon_water = NULL;
    SDL_DestroyTexture(tx_icon_acid); tx_icon_acid = NULL;
    SDL_DestroyTexture(tx_icon_virus); tx_icon_virus = NULL;
    SDL_DestroyTexture(tx_icon_wood); tx_icon_wood = NULL;
    SDL_DestroyTexture(tx_icon_metal); tx_icon_metal = NULL;
    SDL_DestroyTexture(tx_icon_eraser); tx_icon_eraser = NULL;
    SDL_DestroyTexture(tx_icon_reset); tx_icon_reset = NULL;
    SDL_DestroyTexture(tx_icon_sand_selected); tx_icon_sand_selected = NULL;
    SDL_DestroyTexture(tx_icon_water_selected); tx_icon_water_selected = NULL;
    SDL_DestroyTexture(tx_icon_acid_selected); tx_icon_acid_selected = NULL;
    SDL_DestroyTexture(tx_icon_virus_selected); tx_icon_virus_selected = NULL;
    SDL_DestroyTexture(tx_icon_wood_selected); tx_icon_wood_selected = NULL;
    SDL_DestroyTexture(tx_icon_metal_selected); tx_icon_metal_selected = NULL;
    SDL_DestroyTexture(tx_icon_eraser_selected); tx_icon_eraser_selected = NULL;
    SDL_DestroyTexture(tx_icon_reset_selected); tx_icon_reset_selected = NULL;
    TTF_CloseFont(ttf_cascadia_code); ttf_cascadia_code = NULL;
    SDL_DestroyTexture(tx_1); tx_1 = NULL;
    SDL_DestroyTexture(tx_2); tx_2 = NULL;
    SDL_DestroyTexture(tx_3); tx_3 = NULL;
    SDL_DestroyTexture(tx_4); tx_4 = NULL;
    SDL_DestroyTexture(tx_5); tx_5 = NULL;
    SDL_DestroyTexture(tx_6); tx_6 = NULL;
    SDL_DestroyTexture(tx_0); tx_0 = NULL;
    SDL_DestroyTexture(tx_c); tx_c = NULL;
    SDL_DestroyTexture(tx_selected_material); tx_selected_material = NULL;
    SDL_DestroyTexture(tx_advanced_options); tx_advanced_options = NULL;
}