#include "global.h"
#include "particle.h"
#include "sprite.h"
#include "screen.h"

void draw_screen();
void update_grid_size();

button_t button_sand;
button_t button_water;
button_t button_acid;
button_t button_virus;
button_t button_wood;
button_t button_metal;
button_t button_eraser;
button_t button_reset;

char str_fps_counter[10];
char str_particle_count[21];
int32_t length_selected_material = 4;

bool show_advanced_options = false;
bool show_static_particles = false;
bool show_fps = true;
bool show_particle_count = false;

void draw_screen()
{
    button_sand = (button_t){ .selected = (current_material.type == SAND), .rect = (SDL_Rect){ window_width - 48, 0, 48, 48 }, .texture = tx_icon_sand, .selected_texture = tx_icon_sand_selected };
    button_water = (button_t){ .selected = (current_material.type == WATER), .rect = (SDL_Rect){ window_width - 48, 48, 48, 48 }, .texture = tx_icon_water, .selected_texture = tx_icon_water_selected };
    button_acid = (button_t){ .selected = (current_material.type == ACID), .rect = (SDL_Rect){ window_width - 48, 96, 48, 48 }, .texture = tx_icon_acid, .selected_texture = tx_icon_acid_selected };
    button_virus = (button_t){ .selected = (current_material.type == VIRUS), .rect = (SDL_Rect){ window_width - 48, 144, 48, 48 }, .texture = tx_icon_virus, .selected_texture = tx_icon_virus_selected };
    button_wood = (button_t){ .selected = (current_material.type == WOOD), .rect = (SDL_Rect){ window_width - 48, 192, 48, 48 }, .texture = tx_icon_wood, .selected_texture = tx_icon_wood_selected };
    button_metal = (button_t){ .selected = (current_material.type == METAL), .rect = (SDL_Rect){ window_width - 48, 240, 48, 48 }, .texture = tx_icon_metal, .selected_texture = tx_icon_metal_selected };
    button_eraser = (button_t){ .selected = (current_material.type == NONE), .rect = (SDL_Rect){ window_width - 48, window_height - 96, 48, 48 }, .texture = tx_icon_eraser, .selected_texture = tx_icon_eraser_selected };
    button_reset = (button_t){ .selected = 0, .rect = (SDL_Rect){ window_width - 48, window_height - 48, 48, 48 }, .texture = tx_icon_reset, .selected_texture = tx_icon_reset_selected };

    SDL_Surface* surface_map = SDL_CreateRGBSurface(0, grid_width, grid_height, 32, 0, 0, 0, 0);

    for (int32_t i = 0; i < grid_width; i++)
    for (int32_t j = 0; j < grid_height; j++)
    {
        SDL_Color color;
        if (cell_grid[i][j].enabled)
        {
            if (show_static_particles && cell_grid[i][j].is_static)
                color = (SDL_Color){ 0xFF, 0x00, 0x00, 0xFF };
            else
                color = cell_grid[i][j].material.color;

            SDL_FillRect(surface_map, &(SDL_Rect){ i, j, 1, 1 }, (color.r << 16) + (color.g << 8) + color.b);
        }
    }

    if (step % 100 == 0)
    {
        if (tx_fps_counter)
        {
            SDL_DestroyTexture(tx_fps_counter);
            tx_fps_counter = NULL;
        }
        if (tx_particle_count)
        {
            SDL_DestroyTexture(tx_particle_count);
            tx_particle_count = NULL;
        }

        snprintf(str_fps_counter, 10, "fps: %d", frames_per_second);
        create_text(str_fps_counter, ttf_cascadia_code, white, &tx_fps_counter);
        snprintf(str_particle_count, 21, "active cells: %d", particle_count);
        create_text(str_particle_count, ttf_cascadia_code, white, &tx_particle_count);
    }

    SDL_Texture* texture_map = SDL_CreateTextureFromSurface(renderer, surface_map);
    SDL_FreeSurface(surface_map); surface_map = NULL;

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    SDL_Point mouse_pos;
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

    SDL_RenderClear(renderer);
    {
        SDL_RenderCopy(renderer, texture_map, NULL, &(SDL_Rect){ 0, 0, grid_width * SCALE, grid_height * SCALE });
        SDL_RenderCopy(renderer, tx_selected_material, NULL, &(SDL_Rect){ 10, window_height - 32, 12 * length_selected_material, 24 });

        SDL_RenderCopy(renderer, button_sand.selected ? button_sand.selected_texture : button_sand.texture, NULL, &button_sand.rect);
        SDL_RenderCopy(renderer, button_water.selected ? button_water.selected_texture : button_water.texture, NULL, &button_water.rect);
        SDL_RenderCopy(renderer, button_acid.selected ? button_acid.selected_texture : button_acid.texture, NULL, &button_acid.rect);
        SDL_RenderCopy(renderer, button_virus.selected ? button_virus.selected_texture : button_virus.texture, NULL, &button_virus.rect);
        SDL_RenderCopy(renderer, button_wood.selected ? button_wood.selected_texture : button_wood.texture, NULL, &button_wood.rect);
        SDL_RenderCopy(renderer, button_metal.selected ? button_metal.selected_texture : button_metal.texture, NULL, &button_metal.rect);
        SDL_RenderCopy(renderer, button_eraser.selected ? button_eraser.selected_texture : button_eraser.texture, NULL, &button_eraser.rect);
        SDL_RenderCopy(renderer, button_reset.texture, NULL, &button_reset.rect);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawRect(renderer, &(SDL_Rect){ window_width - 48, 288, 48, window_height - 384});
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

        SDL_RenderCopy(renderer, tx_1, NULL, &(SDL_Rect){ window_width - 63, 12, 12, 24 });
        SDL_RenderCopy(renderer, tx_2, NULL, &(SDL_Rect){ window_width - 63, 60, 12, 24 });
        SDL_RenderCopy(renderer, tx_3, NULL, &(SDL_Rect){ window_width - 63, 108, 12, 24 });
        SDL_RenderCopy(renderer, tx_4, NULL, &(SDL_Rect){ window_width - 63, 156, 12, 24 });
        SDL_RenderCopy(renderer, tx_5, NULL, &(SDL_Rect){ window_width - 63, 204, 12, 24 });
        SDL_RenderCopy(renderer, tx_6, NULL, &(SDL_Rect){ window_width - 63, 252, 12, 24 });
        SDL_RenderCopy(renderer, tx_0, NULL, &(SDL_Rect){ window_width - 63, window_height - 84, 12, 24 });
        SDL_RenderCopy(renderer, tx_c, NULL, &(SDL_Rect){ window_width - 63, window_height - 36, 12, 24 });

        if (show_advanced_options)
            SDL_RenderCopy(renderer, tx_advanced_options, NULL, &(SDL_Rect){ 150, 75, 372, 192 });
        if (show_fps)
            SDL_RenderCopy(renderer, tx_fps_counter, NULL, &(SDL_Rect){ 10, 10, 12 * strlen(str_fps_counter), 24 });
        if (show_particle_count)
            SDL_RenderCopy(renderer, tx_particle_count, NULL, &(SDL_Rect){ 10, 36, 8 * strlen(str_particle_count), 18 });
    }
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture_map); texture_map = NULL;
}

void update_grid_size()
{
    SDL_GetWindowSize(window, &window_width, &window_height);

    int32_t grid_width_old = grid_width;
    int32_t grid_height_old = grid_height;

    grid_width = (window_width - 48) / SCALE;
    grid_height = window_height / SCALE;

    cell_grid = realloc(cell_grid, grid_width * sizeof(particle_t*));

    if (grid_width > grid_width_old)
    {
        for (int32_t i = 0; i < grid_width_old; i++)
            cell_grid[i] = realloc(cell_grid[i], grid_height * sizeof(particle_t));

        for (int i = grid_width_old; i < grid_width; i++)
        {
            cell_grid[i] = malloc(grid_height * sizeof(particle_t));
            memset(cell_grid[i], 0, grid_height * sizeof(particle_t));
        }

        if (grid_height > grid_height_old)
            for (int32_t i = 0; i < grid_width_old; i++)
                memset(cell_grid[i] + grid_height_old, 0, (grid_height - grid_height_old) * sizeof(particle_t));
    }
    else
    {
        for (int i = 0; i < grid_width; i++)
            cell_grid[i] = realloc(cell_grid[i], grid_height * sizeof(particle_t));

        if (grid_height > grid_height_old)
            for (int i = 0; i < grid_width; i++)
                memset(cell_grid[i] + grid_height_old, 0, (grid_height - grid_height_old) * sizeof(particle_t));
    }

    for (int32_t i = 0; i < grid_width; i++)
    for (int32_t j = 0; j < grid_height; j++)
    {
        cell_grid[i][j].is_static = 0;
    }
}
