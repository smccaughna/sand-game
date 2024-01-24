#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern bool init_game();
extern void update_game();

const extern SDL_Color black;
const extern SDL_Color white;

const extern int32_t max_framerate;
extern int32_t frames_per_second;
extern uint32_t step;

extern bool pause_simulation;
extern bool lock_framerate;