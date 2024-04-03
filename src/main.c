#include "global.h"
#include "input.h"
#include "particle.h"
#include "render.h"
#include "sprite.h"

#define FPS_LIMIT 200

int main(int argc, char** argv)
{
    srand(time(NULL));

    state_t state = { 0 };

    if (!init_sdl() || !init_game(&state)) {
        return 1;
    }

    while (!state.should_quit) {
        state.time.now = SDL_GetTicks64();
        state.time.tick++;

        handle_input(&state);
        update_grid(&state);
        render(&state);

        if (state.options.limit_fps) {
            SDL_Delay((int)(1000.0f / FPS_LIMIT) - (SDL_GetTicks64() - state.time.now));
        }
    }

    quit_game(&state);
    quit_sdl();

    return 0;
}

uint32_t calculate_fps(uint32_t interval, void* param)
{
    ((state_t*)param)->time.fps = (uint32_t)((float)(((state_t*)param)->time.tick - ((state_t*)param)->time.last_tick) / ((float)interval / 1000.0f));
    ((state_t*)param)->time.last_tick = ((state_t*)param)->time.tick;

    return 1000;
}