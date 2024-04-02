#include "global.h"
#include "sprite.h"

#define GLYPH_ATLAS_SIZE 256

const SDL_Color white = { 0xFF, 0xFF, 0xFF, 0xFF };
const SDL_Color black = { 0x00, 0x00, 0x00, 0xFF };

static bool load_texture(SDL_Renderer* renderer, const char* path, SDL_Texture** tex);
static bool load_font(state_t* state, const char* path, int32_t size);

bool load_media(state_t* state)
{
    return (
            load_texture(state->renderer, "res/sprites/atlas.png", &state->sprites.icon_atlas) &&
            load_font(state, "res/fonts/CascadiaMono.ttf", 18)
    );
}

void unload_media(state_t* state)
{
    SDL_DestroyTexture(state->sprites.icon_atlas);
    state->sprites.icon_atlas = NULL;
    SDL_DestroyTexture(state->font.atlas);
    state->font.atlas = NULL;
    TTF_CloseFont(state->font.ttf);
    state->font.ttf = NULL;
}

static bool load_texture(SDL_Renderer* renderer, const char* path, SDL_Texture** tex)
{
    *tex = IMG_LoadTexture(renderer, path);
    if (*tex == NULL)
    {
        printf("Texture brokey: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

static bool load_font(state_t* state, const char* path, int32_t size)
{
    state->font.ttf = TTF_OpenFont(path, size);
    if (state->font.ttf == NULL) {
        printf("Font brokey: %s\n", TTF_GetError());
        return false;
    }

    SDL_Surface* surface = SDL_CreateRGBSurface(0, GLYPH_ATLAS_SIZE, GLYPH_ATLAS_SIZE, 32, 0, 0, 0, 0);
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    SDL_Rect dest = { 0 };

    char c[2];
    c[1] = '\0';

    for (char i = ' '; i < '~'; i++) {
        c[0] = i;

        SDL_Surface* char_surface = TTF_RenderUTF8_Blended(state->font.ttf, c, white);
        TTF_SizeText(state->font.ttf, c, &dest.w, &dest.h);

        if (dest.x + dest.w > GLYPH_ATLAS_SIZE) {
            dest.x = 0;
            dest.y += dest.h + 1;
            if (dest.y + dest.h >= GLYPH_ATLAS_SIZE) {
                printf("Glyph brokey");
                return false;
            }
        }

        SDL_BlitSurface(char_surface, NULL, surface, &dest);
        state->font.glyphs[i] = dest;
        SDL_FreeSurface(char_surface);

        dest.x += dest.w;
    }

    state->font.atlas = SDL_CreateTextureFromSurface(state->renderer, surface);
    SDL_FreeSurface(surface);

    return true;
}