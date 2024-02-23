#include "global.h"
#include "sprite.h"
#include "particle.h"

bool load_media();
void unload_media();
void update_material_text(const char*, int32_t);
void update_fps_count(int32_t);
void update_particle_count(int32_t);

static bool load_texture(const char*, SDL_Texture**);
static bool load_font(const char*, TTF_Font**, int32_t);
static bool create_text(const char*, TTF_Font*, SDL_Color, SDL_Texture**);
static bool update_text(const char*, TTF_Font*, SDL_Color, SDL_Texture**);

SDL_Texture* tx_icon_sand = NULL;
SDL_Texture* tx_icon_water = NULL;
SDL_Texture* tx_icon_acid = NULL;
SDL_Texture* tx_icon_virus = NULL;
SDL_Texture* tx_icon_wood = NULL;
SDL_Texture* tx_icon_metal = NULL;
SDL_Texture* tx_icon_eraser = NULL;
SDL_Texture* tx_icon_reset = NULL;

SDL_Texture* tx_icon_sand_selected = NULL;
SDL_Texture* tx_icon_water_selected = NULL;
SDL_Texture* tx_icon_acid_selected = NULL;
SDL_Texture* tx_icon_virus_selected = NULL;
SDL_Texture* tx_icon_wood_selected = NULL;
SDL_Texture* tx_icon_metal_selected = NULL;
SDL_Texture* tx_icon_eraser_selected = NULL;
SDL_Texture* tx_icon_reset_selected = NULL;

TTF_Font* ttf_cascadia_code = NULL;

SDL_Texture* tx_1 = NULL;
SDL_Texture* tx_2 = NULL;
SDL_Texture* tx_3 = NULL;
SDL_Texture* tx_4 = NULL;
SDL_Texture* tx_5 = NULL;
SDL_Texture* tx_6 = NULL;
SDL_Texture* tx_0 = NULL;
SDL_Texture* tx_c = NULL;

SDL_Texture* tx_fps_count = NULL;
SDL_Texture* tx_particle_count = NULL;
SDL_Texture* tx_selected_material = NULL;
SDL_Texture* tx_advanced_options = NULL;

int32_t length_fps_count = 0;
int32_t length_particle_count = 0;
int32_t length_selected_material = 0;

char* str_advanced_options = "Advanced options:\n"
                             "\n"
                             "[F1]: toggle this menu\n"
                             "[F2]: toggle framerate limit\n"
                             "[F3]: pause/unpause simulation\n"
                             "[F4]: show static particles\n"
                             "[F5]: show FPS\n"
                             "[F6]: show particle count";

static bool load_texture(const char* file, SDL_Texture** tex)
{
    *tex = IMG_LoadTexture(renderer, file);
    if (*tex == NULL)
    {
        printf("Error loading texture %s: %s\n", file, IMG_GetError());
        return false;
    }

    return true;
}

static bool load_font(const char* file, TTF_Font** font, int32_t size)
{
    *font = TTF_OpenFont(file, size);
    if (*font == NULL)
    {
        printf("Error loading font %s: %s\n", file, TTF_GetError());
        return false;
    }

    return true;
}

static bool create_text(const char* text, TTF_Font* font, SDL_Color color, SDL_Texture** tex)
{
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text, color, 0);
    if (surface == NULL)
    {
        printf("Failed to create surface: %s\n", TTF_GetError());
        return false;
    }

    *tex = SDL_CreateTextureFromSurface(renderer, surface);
    if (*tex == NULL)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
        return false; 
    }

    SDL_FreeSurface(surface);

    return true;
}

static bool update_text(const char* text, TTF_Font* font, SDL_Color color, SDL_Texture** tex)
{
    SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(font, text, color, 0);
    if (text_surface == NULL)
    {
        printf("Failed to create surface: %s\n", SDL_GetError());
        return false;
    }

    if (*tex == NULL)
    {
        *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, text_surface->w, text_surface->h);
        if (*tex == NULL)
        {
            printf("Failed to create texture: %s\n", SDL_GetError());
            SDL_FreeSurface(text_surface);
            return false;
        }
    }
    else
    {
        SDL_Point tex_size;
        SDL_QueryTexture(*tex, NULL, NULL, &tex_size.x, &tex_size.y);

        if (tex_size.x != text_surface->w || tex_size.y != text_surface->h)
        {
            SDL_DestroyTexture(*tex);
            *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, text_surface->w, text_surface->h);
            if (*tex == NULL)
            {
                printf("Failed to create texture: %s\n", SDL_GetError());
                SDL_FreeSurface(text_surface);
                return false;
            }
        }
    }

    SDL_Surface* display_surface;
    SDL_LockTextureToSurface(*tex, NULL, &display_surface);
    SDL_FillRect(display_surface, NULL, 0);
    SDL_BlitSurface(text_surface, NULL, display_surface, NULL);
    SDL_UnlockTexture(*tex);

    SDL_FreeSurface(text_surface);

    return true;
}

bool load_media()
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
    SDL_DestroyTexture(tx_fps_count); tx_fps_count = NULL;
    SDL_DestroyTexture(tx_particle_count); tx_particle_count = NULL;
}

void update_material_text(const char* str, int32_t len)
{
    if (tx_selected_material != NULL)
    {
        SDL_DestroyTexture(tx_selected_material);
        tx_selected_material = NULL;
    }

    length_selected_material = len;
    create_text(str, ttf_cascadia_code, white, &tx_selected_material);
}

void update_fps_count(int32_t count)
{
    char str_fps_count[64];
    snprintf(str_fps_count, 64, "fps: %d", frames_per_second);
    length_fps_count = strlen(str_fps_count);
    update_text(str_fps_count, ttf_cascadia_code, white, &tx_fps_count);
}

void update_particle_count(int32_t count)
{
    char str_particle_count[64];
    snprintf(str_particle_count, 64, "active cells: %d", particle_count);
    length_particle_count = strlen(str_particle_count);
    update_text(str_particle_count, ttf_cascadia_code, white, &tx_particle_count);
}