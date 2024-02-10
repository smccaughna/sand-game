#include "global.h"
#include "particle.h"
#include "input.h"
#include "sprite.h"
#include "screen.h"

void update_game();
void cleanup();

static uint32_t calculate_fps(uint32_t, void*);

const int32_t max_framerate = 120;
int32_t frames_per_second = 0;
uint32_t step = 0;

bool lock_framerate = false;
bool pause_simulation = false;

SDL_TimerID fps_timer;

int main(int argc, char** argv)
{
    atexit(cleanup);

    if (!init_game() || !load_media())
        exit(-1);

    cell_grid = malloc(grid_width * sizeof(particle_t*));
    for (int i = 0; i < grid_width; i++)
    {
        cell_grid[i] = malloc(grid_height * sizeof(particle_t));
        memset(cell_grid[i], 0, grid_height * sizeof(particle_t));
    }

    fps_timer = SDL_AddTimer(1000, calculate_fps, NULL);

    while (true)
        update_game();

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

void cleanup()
{
    if (cell_grid != NULL)
    {
        for (int i = 0; i < grid_width; i++)
            free(cell_grid[i]);

        free(cell_grid);
    }

    SDL_RemoveTimer(fps_timer);

    unload_media();

    SDL_DestroyRenderer(renderer); renderer = NULL;
    SDL_DestroyWindow(window); window = NULL;

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

static uint32_t calculate_fps(uint32_t interval, void* param)
{
    frames_per_second = step / (interval / 1000.0f);
    step = 0;

    return 1000;
}
