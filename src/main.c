#include "global.h"
#include "particle.h"
#include "input.h"
#include "sprite.h"
#include "screen.h"

void update_game();
void close_game();
static uint32_t calculate_fps(uint32_t, void*);

const int32_t max_framerate = 120;

int32_t frames_per_second = 0;

int32_t lock_framerate = 1;
int32_t pause_simulation = 0;
uint32_t step = 0;
int32_t quit = 0;

SDL_TimerID fps_timer;

int main(int argc, char** argv)
{
    if (init_game() < 0 || load_media(renderer) < 0)
        close_game();

    cell_grid = (particle_t**)malloc(grid_width * sizeof(particle_t*));
    for (int32_t i = 0; i < grid_width; i++)
    {
        cell_grid[i] = (particle_t*)malloc(grid_height * sizeof(particle_t));
        memset(cell_grid[i], 0, grid_height * sizeof(particle_t));
    }

    fps_timer = SDL_AddTimer(1000, calculate_fps, NULL);

    while (!quit)
        update_game(window, renderer);

    return 0;
}

void update_game()
{
    uint32_t start = SDL_GetTicks();

    step++;

    handle_input();

    if (!pause_simulation)
        update_grid();

    draw_screen();

    uint64_t end = SDL_GetTicks();
    float milliseconds_elapsed = end - start;
    float delay = floor((1000.0f / (float)max_framerate) - milliseconds_elapsed);
    if (delay > 0 && lock_framerate)
        SDL_Delay(delay);
}

void close_game()
{
    for (int32_t i = 0; i < grid_width; i++)
        free(cell_grid[i]);

    free(cell_grid);

    SDL_RemoveTimer(fps_timer);

    unload_media();

    SDL_DestroyRenderer(renderer); renderer = NULL;
    SDL_DestroyWindow(window); window = NULL;

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();

    exit(0);
}

static uint32_t calculate_fps(uint32_t interval, void* param)
{
    frames_per_second = step / (interval / 1000.0f);
    step = 0;

    return 1000;
}