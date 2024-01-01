#include "global.h"
#include "particle.h"

static void update_sand(int32_t, int32_t);
static void update_water(int32_t, int32_t);
static void update_acid(int32_t, int32_t);
static void update_virus(int32_t, int32_t);

void update_grid();
void clear_grid();
void activate_neighbors(int32_t, int32_t);

const SDL_Color color_sand = { 0xC2, 0xB2, 0x80, 0xFF };
const SDL_Color color_water = { 0x74, 0xAA, 0xC4, 0xFF };
const SDL_Color color_acid = { 0x89, 0xFF, 0x00, 0xFF };
const SDL_Color color_virus = { 0xBB, 0x00, 0xBB, 0xFF };
const SDL_Color color_wood = { 0x5C, 0x40, 0x33, 0xFF };
const SDL_Color color_metal = { 0x80, 0x80, 0x80, 0xFF };

const material_t sand = { SAND, { 0xC2, 0xB2, 0x80, 0xFF } };
const material_t water = { WATER, { 0x74, 0xAA, 0xC4, 0xFF } };
const material_t acid = { ACID, { 0x89, 0xFF, 0x00, 0xFF } };
const material_t virus = { VIRUS, { 0xBB, 0x00, 0xBB, 0xFF } };
const material_t wood = { WOOD, { 0x5C, 0x40, 0x33, 0xFF } };
const material_t metal = { METAL, { 0x80, 0x80, 0x80, 0xFF } };
const material_t none = { NONE, { 0 } };

particle_t** cell_grid;

int32_t grid_width = 616;
int32_t grid_height = 360;

material_t current_material;

int32_t particle_count;

void update_grid()
{
    particle_count = 0;

    for (int32_t i = 0; i < grid_width; i++)
    for (int32_t j = 0; j < grid_height; j++)
    {
        if (cell_grid[i][j].enabled && cell_grid[i][j].update_step != step)
        {
            if (!cell_grid[i][j].is_static)
            {
                switch (cell_grid[i][j].material.type)
                {
                case SAND:
                    update_sand(i, j);
                    break;
                case WATER:
                    update_water(i, j);
                    break;
                case ACID:
                    update_acid(i, j);
                    break;
                case VIRUS:
                    update_virus(i, j);
                case WOOD:
                    break;
                }
            }
        }
    }

    for (int32_t i = 0; i < grid_width; i++)
    for (int32_t j = 0; j < grid_height; j++)
        if (cell_grid[i][j].enabled)
            particle_count++;
}

void clear_grid()
{
    for (int32_t i = 0; i < grid_width; i++)
        memset(cell_grid[i], 0, grid_height * sizeof(particle_t));
}

static void update_sand(int32_t x, int32_t y)
{
    int32_t direction = (step % 2) ? 1 : -1;
    int32_t boundary = (step % 2) ? grid_width - 1 : 0;
    int32_t alt_boundary = (step % 2) ? 0 : grid_width - 1;

    if (cell_grid[x][y + 1].enabled && cell_grid[x][y + 1].material.type == WATER)
    {
        particle_t sand_p = cell_grid[x][y];

        cell_grid[x][y] = cell_grid[x][y + 1];
        cell_grid[x][y].update_step = step;
        cell_grid[x][y + 1] = sand_p;
        cell_grid[x][y + 1].update_step = step;
        activate_neighbors(x, y);
    }
    else if (x != boundary && cell_grid[x + direction][y + 1].enabled && cell_grid[x + direction][y + 1].material.type == WATER)
    {
        particle_t sand_p = cell_grid[x][y];

        cell_grid[x][y] = cell_grid[x + direction][y + 1];
        cell_grid[x][y].update_step = step;
        cell_grid[x + direction][y + 1] = sand_p;
        cell_grid[x + direction][y + 1].update_step = step;
        activate_neighbors(x, y);
    }
    else if (x != alt_boundary && cell_grid[x - direction][y + 1].enabled && cell_grid[x - direction][y + 1].material.type == WATER)
    {
        particle_t sand_p = cell_grid[x][y];

        cell_grid[x][y] = cell_grid[x - direction][y + 1];
        cell_grid[x][y].update_step = step;
        cell_grid[x - direction][y + 1] = sand_p;
        cell_grid[x - direction][y + 1].update_step = step;
        activate_neighbors(x, y);
    }

    if (!cell_grid[x][y].enabled) return;

    if (y < grid_height - 1 && !cell_grid[x][y + 1].enabled)
    {
        cell_grid[x][y + 1] = cell_grid[x][y];
        cell_grid[x][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != boundary && y < grid_height - 1 && !cell_grid[x + direction][y + 1].enabled)
    {
        cell_grid[x + direction][y + 1] = cell_grid[x][y];
        cell_grid[x + direction][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != alt_boundary && y < grid_height - 1 && !cell_grid[x - direction][y + 1].enabled)
    {
        cell_grid[x - direction][y + 1] = cell_grid[x][y];
        cell_grid[x - direction][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else
    {
        cell_grid[x][y].is_static = 1;
    }
}

static void update_water(int32_t x, int32_t y)
{
    int32_t direction = (step % 2) ? 1 : -1;
    int32_t boundary = (step % 2) ? grid_width - 1 : 0;
    int32_t alt_boundary = (step % 2) ? 0 : grid_width - 1;

    if (y < grid_height - 1 && !cell_grid[x][y + 1].enabled)
    {
        cell_grid[x][y + 1] = cell_grid[x][y];
        cell_grid[x][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != boundary && y < grid_height - 1 && !cell_grid[x + direction][y + 1].enabled)
    {
        cell_grid[x + direction][y + 1] = cell_grid[x][y];
        cell_grid[x + direction][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != alt_boundary && y < grid_height - 1 && !cell_grid[x - direction][y + 1].enabled)
    {
        cell_grid[x - direction][y + 1] = cell_grid[x][y];
        cell_grid[x - direction][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != boundary && !cell_grid[x + direction][y].enabled)
    {
        cell_grid[x + direction][y] = cell_grid[x][y];
        cell_grid[x + direction][y].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != alt_boundary && !cell_grid[x - direction][y].enabled)
    {
        cell_grid[x - direction][y] = cell_grid[x][y];
        cell_grid[x - direction][y].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else
    {
        cell_grid[x][y].is_static = 1;
    }
}

static void update_acid(int32_t x, int32_t y)
{
    int32_t direction = (step % 2) ? 1 : -1;
    int32_t boundary = (step % 2) ? grid_width - 1 : 0;
    int32_t alt_boundary = (step % 2) ? 0 : grid_width - 1;

    if (y < grid_height - 1 && cell_grid[x][y + 1].enabled && cell_grid[x][y + 1].material.type == WATER)
    {
        particle_t acid_p = cell_grid[x][y];
        
        cell_grid[x][y] = cell_grid[x][y + 1];
        cell_grid[x][y].update_step = step;
        cell_grid[x][y + 1] = acid_p;
        cell_grid[x][y + 1].update_step = step;
        activate_neighbors(x, y);
    }
    else if (y < grid_height - 1 && x != boundary && cell_grid[x + direction][y + 1].enabled && cell_grid[x + direction][y + 1].material.type == WATER)
    {
        particle_t acid_p = cell_grid[x][y];
        
        cell_grid[x][y] = cell_grid[x + direction][y + 1];
        cell_grid[x][y].update_step = step;
        cell_grid[x + direction][y + 1] = acid_p;
        cell_grid[x + direction][y + 1].update_step = step;
        activate_neighbors(x, y);
    }
    else if (y < grid_height - 1 && x != alt_boundary && cell_grid[x - direction][y + 1].enabled && cell_grid[x - direction][y + 1].material.type == WATER)
    {
        particle_t acid_p = cell_grid[x][y];
        
        cell_grid[x][y] = cell_grid[x - direction][y + 1];
        cell_grid[x][y].update_step = step;
        cell_grid[x - direction][y + 1] = acid_p;
        cell_grid[x - direction][y + 1].update_step = step;
        activate_neighbors(x, y);
    }
    else if (y < grid_height - 1 && x != boundary && cell_grid[x + direction][y].enabled && cell_grid[x + direction][y].material.type == WATER)
    {
        particle_t acid_p = cell_grid[x][y];
        
        cell_grid[x][y] = cell_grid[x + direction][y];
        cell_grid[x][y].update_step = step;
        cell_grid[x + direction][y] = acid_p;
        cell_grid[x + direction][y].update_step = step;
        activate_neighbors(x, y);
    }
    else if (y < grid_height - 1 && x != alt_boundary && cell_grid[x - direction][y].enabled && cell_grid[x - direction][y].material.type == WATER)
    {
        particle_t acid_p = cell_grid[x][y];
        
        cell_grid[x][y] = cell_grid[x - direction][y];
        cell_grid[x][y].update_step = step;
        cell_grid[x - direction][y] = acid_p;
        cell_grid[x - direction][y].update_step = step;
        activate_neighbors(x, y);
    }
    else if (y < grid_height - 1 && cell_grid[x][y + 1].enabled && cell_grid[x][y + 1].material.type != ACID && cell_grid[x][y + 1].material.type != WATER && cell_grid[x][y + 1].material.type != METAL)
    {
        if (rand() < RAND_MAX / 10)
        {
            cell_grid[x][y + 1].enabled = 0;
            cell_grid[x][y].enabled = 0;
            activate_neighbors(x, y);
        }
    }
    else if (y < 0 && cell_grid[x][y - 1].enabled && cell_grid[x][y - 1].material.type != ACID && cell_grid[x][y - 1].material.type != WATER && cell_grid[x][y - 1].material.type != METAL)
    {
        if (rand() < RAND_MAX / 10)
        {
            cell_grid[x][y - 1].enabled = 0;
            cell_grid[x][y].enabled = 0;
            activate_neighbors(x, y);
        }
    }
    else if (x != boundary && cell_grid[x + direction][y].enabled && cell_grid[x + direction][y].material.type != ACID && cell_grid[x + direction][y].material.type != WATER && cell_grid[x + direction][y].material.type != METAL)
    {
        if (rand() < RAND_MAX / 10)
        {
            cell_grid[x + direction][y].enabled = 0;
            cell_grid[x][y].enabled = 0;
            activate_neighbors(x, y);
        }
    }
    else if (x != alt_boundary && cell_grid[x - direction][y].enabled && cell_grid[x - direction][y].material.type != ACID && cell_grid[x - direction][y].material.type != WATER && cell_grid[x - direction][y].material.type != METAL)
    {
        if (rand() < RAND_MAX / 10)
        {
            cell_grid[x - direction][y].enabled = 0;
            cell_grid[x][y].enabled = 0;
            activate_neighbors(x, y);
        }
    }
    else if (y < grid_height - 1 && !cell_grid[x][y + 1].enabled)
    {
        cell_grid[x][y + 1] = cell_grid[x][y];
        cell_grid[x][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != boundary && y < grid_height - 1 && !cell_grid[x + direction][y + 1].enabled)
    {
        cell_grid[x + direction][y + 1] = cell_grid[x][y];
        cell_grid[x + direction][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != alt_boundary && y < grid_height - 1 && !cell_grid[x - direction][y + 1].enabled)
    {
        cell_grid[x - direction][y + 1] = cell_grid[x][y];
        cell_grid[x - direction][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != boundary && !cell_grid[x + direction][y].enabled)
    {
        cell_grid[x + direction][y] = cell_grid[x][y];
        cell_grid[x + direction][y].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != alt_boundary && !cell_grid[x - direction][y].enabled)
    {
        cell_grid[x - direction][y] = cell_grid[x][y];
        cell_grid[x - direction][y].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else
    {
        cell_grid[x][y].is_static = 1;
    }
}

void update_virus(int32_t x, int32_t y)
{
    int32_t direction = (step % 2) ? 1 : -1;
    int32_t boundary = (step % 2) ? grid_width - 1 : 0;
    int32_t alt_boundary = (step % 2) ? 0 : grid_width - 1;

    if (y < grid_height - 1 && cell_grid[x][y + 1].enabled && cell_grid[x][y + 1].material.type == WATER)
    {
        particle_t acid_p = cell_grid[x][y];
        
        cell_grid[x][y] = cell_grid[x][y + 1];
        cell_grid[x][y].update_step = step;
        cell_grid[x][y + 1] = acid_p;
        cell_grid[x][y + 1].update_step = step;
        activate_neighbors(x, y);
    }
    else if (y < grid_height - 1 && x != boundary && cell_grid[x + direction][y + 1].enabled && cell_grid[x + direction][y + 1].material.type == WATER)
    {
        particle_t acid_p = cell_grid[x][y];
        
        cell_grid[x][y] = cell_grid[x + direction][y + 1];
        cell_grid[x][y].update_step = step;
        cell_grid[x + direction][y + 1] = acid_p;
        cell_grid[x + direction][y + 1].update_step = step;
        activate_neighbors(x, y);
    }
    else if (y < grid_height - 1 && x != alt_boundary && cell_grid[x - direction][y + 1].enabled && cell_grid[x - direction][y + 1].material.type == WATER)
    {
        particle_t acid_p = cell_grid[x][y];
        
        cell_grid[x][y] = cell_grid[x - direction][y + 1];
        cell_grid[x][y].update_step = step;
        cell_grid[x - direction][y + 1] = acid_p;
        cell_grid[x - direction][y + 1].update_step = step;
        activate_neighbors(x, y);
    }
    else if (y < grid_height - 1 && cell_grid[x][y + 1].enabled && (cell_grid[x][y + 1].material.type == SAND || cell_grid[x][y + 1].material.type == WOOD))
    {
        if (rand() < RAND_MAX / 25)
        {
            cell_grid[x][y + 1].material = virus;
            cell_grid[x][y + 1].update_step = step;
            cell_grid[x][y].update_step = step;
            activate_neighbors(x, y);
        }
    }
    else if (y > 0 && cell_grid[x][y - 1].enabled && (cell_grid[x][y - 1].material.type == SAND || cell_grid[x][y - 1].material.type == WOOD))
    {
        if (rand() < RAND_MAX / 25)
        {
            cell_grid[x][y - 1].material = virus;
            cell_grid[x][y - 1].update_step = step;
            cell_grid[x][y].update_step = step;
            activate_neighbors(x, y);
        }
    }
    else if (x != boundary && cell_grid[x + direction][y].enabled && (cell_grid[x + direction][y].material.type == SAND || cell_grid[x + direction][y].material.type == WOOD))
    {
        if (rand() < RAND_MAX / 25)
        {
            cell_grid[x + direction][y].material = virus;
            cell_grid[x + direction][y].update_step = step;
            cell_grid[x][y].update_step = step;
            activate_neighbors(x, y);
        }
    }
    else if (x != alt_boundary && cell_grid[x - direction][y].enabled && (cell_grid[x - direction][y].material.type == SAND || cell_grid[x - direction][y].material.type == WOOD))
    {
        if (rand() < RAND_MAX / 25)
        {
            cell_grid[x - direction][y].material = virus;
            cell_grid[x - direction][y].update_step = step;
            cell_grid[x][y].update_step = step;
            activate_neighbors(x, y);
        }
    }
    else if (y <= grid_height - 1 && !cell_grid[x][y + 1].enabled)
    {
        cell_grid[x][y + 1] = cell_grid[x][y];
        cell_grid[x][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != boundary && y <= grid_height - 1 && !cell_grid[x + direction][y + 1].enabled)
    {
        cell_grid[x + direction][y + 1] = cell_grid[x][y];
        cell_grid[x + direction][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else if (x != alt_boundary && y <= grid_height - 1 && !cell_grid[x - direction][y + 1].enabled)
    {
        cell_grid[x - direction][y + 1] = cell_grid[x][y];
        cell_grid[x - direction][y + 1].update_step = step;
        cell_grid[x][y].enabled = 0;
        activate_neighbors(x, y);
    }
    else
    {
        cell_grid[x][y].is_static = 1;
    }
}

void activate_neighbors(int32_t x, int32_t y)
{
    if (y > 0)
    {
        if (x > 0) cell_grid[x - 1][y - 1].is_static = 0;
        cell_grid[x][y - 1].is_static = 0;
        if (x < grid_width - 1) cell_grid[x + 1][y - 1].is_static = 0;
    }

    if (x > 0) cell_grid[x - 1][y].is_static = 0;
    if (x < grid_width - 1) cell_grid[x + 1][y].is_static = 0;

    if (y < grid_height - 1)
    {
        if (x > 0) cell_grid[x - 1][y + 1].is_static = 0;
        cell_grid[x][y + 1].is_static = 0;
        if (x < grid_width - 1) cell_grid[x + 1][y + 1].is_static = 0;
    }
}