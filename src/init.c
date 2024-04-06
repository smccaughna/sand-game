#include "global.h"
#include "particle.h"
#include "render.h"
#include "sprite.h"

bool init_sdl()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL brokey :(\n");
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("SDL_image brokey :(\n");
        return false;
    }

    if (TTF_Init() < 0) {
        printf("SDL_ttf brokey :(\n");
        return false;
    }

    SDL_SetHint(SDL_HINT_TIMER_RESOLUTION, "1");

    return true;
}

bool init_game(state_t* state)
{
    state->window = SDL_CreateWindow("more sand", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!state->window) {
        printf("Window brokey :(\n");
        return false;
    }

    SDL_SetWindowMinimumSize(state->window, 640, 480);

    state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);
    if (!state->renderer) {
        printf("Renderer brokey :(\n");
        return false;
    }

    init_grid(state);
    init_buttons(state);

    state->grid.tex = SDL_CreateTexture(
            state->renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            state->grid.rect.w,
            state->grid.rect.h
    );

    state->time.fps_timer = SDL_AddTimer(1000, calculate_fps, state);

    if (!load_media(state)) {
        printf("Error loading media\n");
        return false;
    }

    state->options.show_fps = true;
    state->options.limit_fps = true;
    state->options.cursor_size = 5;

    return true;
}

void quit_game(state_t* state)
{
    unload_media(state);

    if (state->window) {
        SDL_DestroyWindow(state->window);
    }
    
    if (state->renderer) {
        SDL_DestroyRenderer(state->renderer);
    }

    for (int32_t i = 0; i < state->grid.rect.w; i++) {
        for (int32_t j = 0; j < state->grid.rect.h; j++) {
            if (state->grid.cells[i][j] != NULL) {
                free(state->grid.cells[i][j]);
            }
        }
    }

    for (int32_t i = 0; i < 8; i++) {
        free(state->buttons[i]);
    }

    SDL_DestroyTexture(state->sprites.icon_atlas);
    SDL_DestroyTexture(state->font.atlas);

    TTF_CloseFont(state->font.ttf);

    SDL_RemoveTimer(state->time.fps_timer);
}

void quit_sdl()
{
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}