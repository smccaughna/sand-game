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

extern bool init_game();

extern SDL_Window* window;
extern SDL_Renderer* renderer;

const extern SDL_Color black;
const extern SDL_Color white;

extern uint32_t step;
extern int32_t frames_per_second;