#include "global.h"
#include "particle.h"

const SDL_Color color_sand = { 0xC2, 0xB2, 0x80, 0xFF };
const SDL_Color color_water = { 0x74, 0xAA, 0xC4, 0xFF };
const SDL_Color color_acid = { 0x89, 0xFF, 0x00, 0xFF };
const SDL_Color color_virus = { 0xBB, 0x00, 0xBB, 0xFF };
const SDL_Color color_wood = { 0x5C, 0x40, 0x33, 0xFF };
const SDL_Color color_metal = { 0x80, 0x80, 0x80, 0xFF };

static void update_sand(state_t* state, int32_t x, int32_t y);
static void update_water(state_t* state, int32_t x, int32_t y);
static void update_acid(state_t* state, int32_t x, int32_t y);
static void update_virus(state_t* state, int32_t x, int32_t y);

void update_grid(state_t* state)
{
    for (int32_t j = 0; j < state->grid.rect.h; j++) {
        for (int32_t i = 0; i < state->grid.rect.w; i++) {
            if (state->grid.cells[i][j] != NULL &&
               !state->grid.cells[i][j]->is_static &&
                state->grid.cells[i][j]->last_update != state->time.tick)
            {
                switch (state->grid.cells[i][j]->material.type) {
                    case SAND:  update_sand(state, i, j); break;
                    case WATER: update_water(state, i, j); break;
                    case ACID:  update_acid(state, i, j); break;
                    case VIRUS: update_virus(state, i, j); break;
                    case WOOD:
                    case METAL:
                        break;
                    default:
                        update_sand(state, i, j);
                }
            }
        }
    }
}

static void update_sand(state_t* state, int32_t x, int32_t y)
{
    int32_t direction = (rand() > RAND_MAX / 2) ? 1 : -1;

    if (!check_out_of_bounds(&state->grid.rect, x, y + 1) &&
         state->grid.cells[x][y + 1] != NULL &&
        (state->grid.cells[x][y + 1]->material.type == WATER ||
         state->grid.cells[x][y + 1]->material.type == ACID))
    {
        particle_t* water_p = state->grid.cells[x][y + 1];
        state->grid.cells[x][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y + 1]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y + 1) &&
              state->grid.cells[x + direction][y + 1] != NULL &&
             (state->grid.cells[x + direction][y + 1]->material.type == WATER ||
              state->grid.cells[x + direction][y + 1]->material.type == ACID))
    {
        particle_t* water_p = state->grid.cells[x + direction][y + 1];
        state->grid.cells[x + direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x + direction][y + 1]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y + 1) &&
              state->grid.cells[x - direction][y + 1] != NULL &&
             (state->grid.cells[x - direction][y + 1]->material.type == WATER ||
              state->grid.cells[x - direction][y + 1]->material.type == ACID))
    {
        particle_t* water_p = state->grid.cells[x - direction][y + 1];
        state->grid.cells[x - direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x - direction][y + 1]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x, y + 1) &&
              state->grid.cells[x][y + 1] == NULL)
    {
        state->grid.cells[x][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y + 1) &&
              state->grid.cells[x + direction][y + 1] == NULL)
    {
        state->grid.cells[x + direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x + direction][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y + 1) &&
              state->grid.cells[x - direction][y + 1] == NULL)
    {
        state->grid.cells[x - direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x - direction][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else {
        state->grid.cells[x][y]->is_static = true;
    }
}

static void update_water(state_t* state, int32_t x, int32_t y)
{
    int32_t direction = (rand() > RAND_MAX / 2) ? 1 : -1;

    if (!check_out_of_bounds(&state->grid.rect, x, y + 1) &&
         state->grid.cells[x][y + 1] == NULL)
    {
        state->grid.cells[x][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y + 1) &&
              state->grid.cells[x + direction][y + 1] == NULL)
    {
        state->grid.cells[x + direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x + direction][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y + 1) &&
              state->grid.cells[x - direction][y + 1] == NULL)
    {
        state->grid.cells[x - direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x - direction][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y) &&
              state->grid.cells[x + direction][y] == NULL)
    {
        state->grid.cells[x + direction][y] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x + direction][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y) &&
              state->grid.cells[x - direction][y] == NULL)
    {
        state->grid.cells[x - direction][y] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x - direction][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else {
        state->grid.cells[x][y]->is_static = true;
    }
}

static void update_acid(state_t* state, int32_t x, int32_t y)
{
    int32_t direction = (rand() > RAND_MAX / 2) ? 1 : -1;

    if (!check_out_of_bounds(&state->grid.rect, x, y + 1) &&
        state->grid.cells[x][y + 1] != NULL &&
        state->grid.cells[x][y + 1]->material.type == WATER)
    {
        particle_t* water_p = state->grid.cells[x][y + 1];
        state->grid.cells[x][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y + 1]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y + 1) &&
             state->grid.cells[x + direction][y + 1] != NULL &&
             state->grid.cells[x + direction][y + 1]->material.type == WATER)
    {
        particle_t* water_p = state->grid.cells[x + direction][y + 1];
        state->grid.cells[x + direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x + direction][y + 1]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y + 1) &&
             state->grid.cells[x - direction][y + 1] != NULL &&
             state->grid.cells[x - direction][y + 1]->material.type == WATER)
    {
        particle_t* water_p = state->grid.cells[x - direction][y + 1];
        state->grid.cells[x - direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x - direction][y + 1]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y) &&
              state->grid.cells[x + direction][y] != NULL &&
              state->grid.cells[x + direction][y]->material.type == WATER)
    {
        particle_t* water_p = state->grid.cells[x + direction][y];
        state->grid.cells[x + direction][y] = state->grid.cells[x][y];
        state->grid.cells[x + direction][y]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y) &&
              state->grid.cells[x - direction][y] != NULL &&
              state->grid.cells[x - direction][y]->material.type == WATER)
    {
        particle_t* water_p = state->grid.cells[x - direction][y];
        state->grid.cells[x - direction][y] = state->grid.cells[x][y];
        state->grid.cells[x - direction][y]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x, y - 1) &&
              state->grid.cells[x][y - 1] != NULL &&
              state->grid.cells[x][y - 1]->material.type != ACID &&
              state->grid.cells[x][y - 1]->material.type != WATER &&
              state->grid.cells[x][y - 1]->material.type != METAL)
    {
        if (rand() < RAND_MAX / 15) {
            free(state->grid.cells[x][y]);
            free(state->grid.cells[x][y - 1]);
            state->grid.cells[x][y] = NULL;
            state->grid.cells[x][y - 1] = NULL;
            activate_neighbors(state, x, y);
        }
    }
    else if (!check_out_of_bounds(&state->grid.rect, x, y + 1) &&
              state->grid.cells[x][y + 1] != NULL &&
              state->grid.cells[x][y + 1]->material.type != ACID &&
              state->grid.cells[x][y + 1]->material.type != WATER &&
              state->grid.cells[x][y + 1]->material.type != METAL)
    {
        if (rand() < RAND_MAX / 15) {
            free(state->grid.cells[x][y]);
            free(state->grid.cells[x][y + 1]);
            state->grid.cells[x][y] = NULL;
            state->grid.cells[x][y + 1] = NULL;
            activate_neighbors(state, x, y);
        }
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y) &&
             state->grid.cells[x + direction][y] != NULL &&
             state->grid.cells[x + direction][y]->material.type != ACID &&
             state->grid.cells[x + direction][y]->material.type != WATER &&
             state->grid.cells[x + direction][y]->material.type != METAL)
    {
        if (rand() < RAND_MAX / 15) {
            free(state->grid.cells[x][y]);
            free(state->grid.cells[x + direction][y]);
            state->grid.cells[x][y] = NULL;
            state->grid.cells[x + direction][y] = NULL;
            activate_neighbors(state, x, y);
        }
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y) &&
             state->grid.cells[x - direction][y] != NULL &&
             state->grid.cells[x - direction][y]->material.type != ACID &&
             state->grid.cells[x - direction][y]->material.type != WATER &&
             state->grid.cells[x - direction][y]->material.type != METAL)
    {
        if (rand() < RAND_MAX / 15) {
            free(state->grid.cells[x][y]);
            free(state->grid.cells[x - direction][y]);
            state->grid.cells[x][y] = NULL;
            state->grid.cells[x - direction][y] = NULL;
            activate_neighbors(state, x, y);
        }
    }
    else if (!check_out_of_bounds(&state->grid.rect, x, y + 1) &&
              state->grid.cells[x][y + 1] == NULL)
    {
        state->grid.cells[x][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y + 1) &&
              state->grid.cells[x + direction][y + 1] == NULL)
    {
        state->grid.cells[x + direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x + direction][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y + 1) &&
              state->grid.cells[x - direction][y + 1] == NULL)
    {
        state->grid.cells[x - direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x - direction][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y) &&
              state->grid.cells[x + direction][y] == NULL)
    {
        state->grid.cells[x + direction][y] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x + direction][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y) &&
              state->grid.cells[x - direction][y] == NULL)
    {
        state->grid.cells[x - direction][y] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x - direction][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else {
        state->grid.cells[x][y]->is_static = true;
    }
}

static void update_virus(state_t* state, int32_t x, int32_t y)
{
    int32_t direction = (rand() > RAND_MAX / 2) ? 1 : -1;

    if (!check_out_of_bounds(&state->grid.rect, x, y + 1) &&
        state->grid.cells[x][y + 1] != NULL &&
        (state->grid.cells[x][y + 1]->material.type == WATER ||
         state->grid.cells[x][y + 1]->material.type == ACID))
    {
        particle_t* water_p = state->grid.cells[x][y + 1];
        state->grid.cells[x][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y + 1]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y + 1) &&
             state->grid.cells[x + direction][y + 1] != NULL &&
             (state->grid.cells[x + direction][y + 1]->material.type == WATER ||
              state->grid.cells[x + direction][y + 1]->material.type == ACID))
    {
        particle_t* water_p = state->grid.cells[x + direction][y + 1];
        state->grid.cells[x + direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x + direction][y + 1]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y + 1) &&
             state->grid.cells[x - direction][y + 1] != NULL &&
             (state->grid.cells[x - direction][y + 1]->material.type == WATER ||
              state->grid.cells[x - direction][y + 1]->material.type == ACID))
    {
        particle_t* water_p = state->grid.cells[x - direction][y + 1];
        state->grid.cells[x - direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x - direction][y + 1]->last_update = state->time.tick;
        state->grid.cells[x][y] = water_p;
        state->grid.cells[x][y]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x, y + 1) &&
              state->grid.cells[x][y + 1] != NULL &&
             (state->grid.cells[x][y + 1]->material.type == SAND ||
              state->grid.cells[x][y + 1]->material.type == WOOD))
    {
        if (rand() < RAND_MAX / 15) {
            state->grid.cells[x][y + 1]->material.type = VIRUS;
            state->grid.cells[x][y + 1]->material.color = color_virus;
            state->grid.cells[x][y + 1]->last_update = state->time.tick;
            state->grid.cells[x][y]->last_update = state->time.tick;
            activate_neighbors(state, x, y);
        }
    }
    else if (!check_out_of_bounds(&state->grid.rect, x, y - 1) &&
             state->grid.cells[x][y - 1] != NULL &&
            (state->grid.cells[x][y - 1]->material.type == SAND ||
             state->grid.cells[x][y - 1]->material.type == WOOD))
    {
        if (rand() < RAND_MAX / 15) {
            state->grid.cells[x][y - 1]->material.type = VIRUS;
            state->grid.cells[x][y - 1]->material.color = color_virus;
            state->grid.cells[x][y - 1]->last_update = state->time.tick;
            state->grid.cells[x][y]->last_update = state->time.tick;
            activate_neighbors(state, x, y);
        }
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y) &&
             state->grid.cells[x + direction][y] != NULL &&
            (state->grid.cells[x + direction][y]->material.type == SAND ||
             state->grid.cells[x + direction][y]->material.type == WOOD))
    {
        if (rand() < RAND_MAX / 15) {
            state->grid.cells[x + direction][y]->material.type = VIRUS;
            state->grid.cells[x + direction][y]->material.color = color_virus;
            state->grid.cells[x + direction][y]->last_update = state->time.tick;
            state->grid.cells[x][y]->last_update = state->time.tick;
            activate_neighbors(state, x, y);
        }
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y) &&
             state->grid.cells[x - direction][y] != NULL &&
            (state->grid.cells[x - direction][y]->material.type == SAND ||
             state->grid.cells[x - direction][y]->material.type == WOOD))
    {
        if (rand() < RAND_MAX / 15) {
            state->grid.cells[x - direction][y]->material.type = VIRUS;
            state->grid.cells[x - direction][y]->material.color = color_virus;
            state->grid.cells[x - direction][y]->last_update = state->time.tick;
            state->grid.cells[x][y]->last_update = state->time.tick;
            activate_neighbors(state, x, y);
        }
    }
    else if (!check_out_of_bounds(&state->grid.rect, x, y + 1) &&
              state->grid.cells[x][y + 1] == NULL)
    {
        state->grid.cells[x][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x + direction, y + 1) &&
             state->grid.cells[x + direction][y + 1] == NULL)
    {
        state->grid.cells[x + direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x + direction][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else if (!check_out_of_bounds(&state->grid.rect, x - direction, y + 1) &&
             state->grid.cells[x - direction][y + 1] == NULL)
    {
        state->grid.cells[x - direction][y + 1] = state->grid.cells[x][y];
        state->grid.cells[x][y] = NULL;
        state->grid.cells[x - direction][y + 1]->last_update = state->time.tick;
        activate_neighbors(state, x, y);
    }
    else {
        state->grid.cells[x][y]->is_static = true;
    }
}

void activate_neighbors(state_t* state, int32_t x, int32_t y)
{
    for (int32_t i = -1; i <= 1; i++) {
        for (int32_t j = -1; j <= 1; j++) {
            if (!check_out_of_bounds(&state->grid.rect, x + i, y + j) && state->grid.cells[x + i][y + j] != NULL) {
                state->grid.cells[x + i][y + j]->is_static = false;
            }
        }
    }
}

void init_grid(state_t* state)
{
    SDL_GetWindowSize(state->window, &state->window_rect.w, &state->window_rect.h);
    state->grid.rect = (SDL_Rect){ 0, 0, (state->window_rect.w - 48) / SCALE, state->window_rect.h / SCALE };

    state->grid.cells = malloc(state->grid.rect.w * sizeof(particle_t**));

    for (int32_t i = 0; i < state->grid.rect.w; i++) {
        state->grid.cells[i] = malloc(state->grid.rect.h * sizeof(particle_t*));
        memset(state->grid.cells[i], 0, state->grid.rect.h * sizeof(particle_t*));
    }
}

void clear_grid(state_t* state)
{
    for (int32_t i = 0; i < state->grid.rect.w; i++) {
        for (int32_t j = 0; j < state->grid.rect.h; j++) {
            if (state->grid.cells[i][j] != NULL) {
                free(state->grid.cells[i][j]);
                state->grid.cells[i][j] = NULL;
            }
        }
    }
}

void update_grid_size(state_t* state)
{
    SDL_GetWindowSize(state->window, &state->window_rect.w, &state->window_rect.h);

    SDL_Rect grid_rect_old = state->grid.rect;
    state->grid.rect = (SDL_Rect){ 0, 0, (state->window_rect.w - 48) / SCALE, state->window_rect.h / SCALE };

    if (state->grid.tex != NULL) {
        SDL_DestroyTexture(state->grid.tex);
        state->grid.tex = NULL;
    }

    state->grid.tex = SDL_CreateTexture(
        state->renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        state->grid.rect.w,
        state->grid.rect.h
    );

    if (state->grid.rect.w < grid_rect_old.w) {
        for (int32_t i = state->grid.rect.w; i < grid_rect_old.w; i++) {
            for (int32_t j = 0; j < grid_rect_old.h; j++) {
                if (state->grid.cells[i][j] != NULL) {
                    free(state->grid.cells[i][j]);
                    state->grid.cells[i][j] = NULL;
                }
            }
            free(state->grid.cells[i]);
            state->grid.cells[i] = NULL;
        }
        state->grid.cells = realloc(state->grid.cells, state->grid.rect.w * sizeof(particle_t**));
    } else {
        state->grid.cells = realloc(state->grid.cells, state->grid.rect.w * sizeof(particle_t**));
        for (int32_t i = grid_rect_old.w; i < state->grid.rect.w; i++) {
            state->grid.cells[i] = malloc(state->grid.rect.h * sizeof(particle_t*));
            memset(state->grid.cells[i], 0, state->grid.rect.h * sizeof(particle_t*));
        }
    }

    if (state->grid.rect.h < grid_rect_old.h) {
        for (int32_t i = 0; i < MIN(state->grid.rect.w, grid_rect_old.w); i++) {
            for (int32_t j = state->grid.rect.h; j < grid_rect_old.h; j++) {
                if (state->grid.cells[i][j] != NULL) {
                    free(state->grid.cells[i][j]);
                    state->grid.cells[i][j] = NULL;
                }
            }
            state->grid.cells[i] = realloc(state->grid.cells[i], state->grid.rect.h * sizeof(particle_t*));
        }
    } else {
        for (int32_t i = 0; i < MIN(state->grid.rect.w, grid_rect_old.w); i++) {
            state->grid.cells[i] = realloc(state->grid.cells[i], state->grid.rect.h * sizeof(particle_t*));
            memset(state->grid.cells[i] + grid_rect_old.h, 0, (state->grid.rect.h - grid_rect_old.h) * sizeof(particle_t*));
        }
    }

    for (int32_t i = 0; i < state->grid.rect.w; i++) {
        for (int32_t j = 0; j < state->grid.rect.h; j++) {
            if (state->grid.cells[i][j] != NULL) {
                state->grid.cells[i][j]->is_static = false;
            }
        }
    }
}