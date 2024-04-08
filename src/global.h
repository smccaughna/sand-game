#pragma once

#define SDL_MAIN_HANDLED

#ifdef WIN32
#define main WinMain
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define MIN(x, y) ((x > y) ? y : x)
#define MAX(x, y) ((x > y) ? x : y)

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SCALE 2

typedef struct particle particle_t;
typedef struct button button_t;

typedef struct state {
    SDL_Window*     window;
    SDL_Rect        window_rect;
    SDL_Renderer*   renderer;
    bool            should_quit;
    struct {
        particle_t***   cells;
        SDL_Rect        rect;
        SDL_Texture*    tex;
    }               grid;
    struct {
        uint64_t        tick;
        uint64_t        last_tick;
        uint64_t        now;
        uint32_t        fps;
        SDL_TimerID     fps_timer;
    }               time;
    struct {
        TTF_Font*       ttf;
        SDL_Rect        glyphs[128];
        SDL_Texture*    atlas;
    }               font;
    struct {
        SDL_Texture*    icon_atlas;
    }               sprites;
    struct {
        bool            show_advanced_options;
        bool            pause_game;
        bool            show_fps;
        bool            limit_fps;
        bool            show_particle_count;
        bool            show_static_particles;
        int8_t          selected_material;
        uint8_t         cursor_size;
    }               options;
    SDL_Point       mouse_pos;
    button_t*       buttons[8];
} state_t;

extern bool init_sdl();
extern bool init_game(state_t* state);
extern void quit_sdl();
extern void quit_game(state_t* state);
extern uint32_t calculate_fps(uint32_t interval, void* param);

static inline bool check_out_of_bounds(const SDL_Rect* rect, int32_t x, int32_t y)
{
    return (
        (x < rect->x)              ||
        (x >= (rect->x + rect->w)) ||
        (y < rect->y)              ||
        (y >= (rect->y + rect->h))
    );
}

static inline bool check_collision(const SDL_Rect* r1, const SDL_Rect* r2)
{
    return !(
        (r1->x > (r2->x + r2->w)) ||
        (r2->x > (r1->x + r1->w)) ||
        (r1->y > (r2->y + r2->h)) ||
        (r2->y > (r1->y + r1->h))
    );
}