#include "global.h"
#include "particle.h"
#include "input.h"
#include "sprite.h"
#include "screen.h"

void handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            exit(0);
        if (event.type == SDL_WINDOWEVENT)
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                update_grid_size();
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_1:
                    current_material = sand;
                    update_material_text("sand", 4);
                    break;
                case SDLK_2:
                    current_material = water;
                    update_material_text("water", 5);
                    break;
                case SDLK_3:
                    current_material = acid;
                    update_material_text("acid", 4);
                    break;
                case SDLK_4:
                    current_material = virus;
                    update_material_text("virus", 5);
                    break;
                case SDLK_5:
                    current_material = wood;
                    update_material_text("wood", 4);
                    break;
                case SDLK_6:
                    current_material = metal;
                    update_material_text("metal", 5);
                    break;
                case SDLK_0:
                    current_material = none;
                    update_material_text("erase", 5);
                    break;
                case SDLK_c:
                    clear_grid();
                    break;
                case SDLK_F1:
                    show_advanced_options = !show_advanced_options;
                    break;
                case SDLK_F2:
                    lock_framerate = !lock_framerate;
                    break;
                case SDLK_F3:
                    pause_simulation = !pause_simulation;
                    break;
                case SDLK_F4:
                    show_static_particles = !show_static_particles;
                    break;
                case SDLK_F5:
                    show_fps = !show_fps;
                    break;
                case SDLK_F6:
                    show_particle_count = !show_particle_count;
                    break;
            }
        }
    }
    
    SDL_Point mouse_pos;
    uint32_t mouse_state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    if (SDL_BUTTON(mouse_state) == 1)
    {
        if (SDL_PointInRect(&mouse_pos, &button_sand.rect))
        {
            current_material = sand;
            update_material_text("sand", 4);
        }
        else if (SDL_PointInRect(&mouse_pos, &button_water.rect))
        {
            current_material = water;
            update_material_text("water", 5);
        }
        else if (SDL_PointInRect(&mouse_pos, &button_acid.rect))
        {
            current_material = acid;
            update_material_text("acid", 4);
        }
        else if (SDL_PointInRect(&mouse_pos, &button_virus.rect))
        {
            current_material = virus;
            update_material_text("virus", 5);
        }
        else if (SDL_PointInRect(&mouse_pos, &button_wood.rect))
        {
            current_material = wood;
            update_material_text("wood", 4);
        }
        else if (SDL_PointInRect(&mouse_pos, &button_metal.rect))
        {
            current_material = metal;
            update_material_text("metal", 5);
        }
        else if (SDL_PointInRect(&mouse_pos, &button_eraser.rect))
        {
            current_material = none;
            update_material_text("sand", 4);
        }
        else if (SDL_PointInRect(&mouse_pos, &button_reset.rect))
            clear_grid();
        else
        {
            for (int32_t i = (mouse_pos.x / SCALE - 2); i <= (mouse_pos.x / SCALE + 2); i++)
            for (int32_t j = (mouse_pos.y / SCALE - 2); j <= (mouse_pos.y / SCALE + 2); j++)
            {
                if (i > 0 && j > 0 && i < grid_width && j < grid_height)
                {
                    cell_grid[i][j].material = current_material;
                    cell_grid[i][j].is_static = 0;
                    activate_neighbors(i, j);

                    if (current_material.type == NONE)
                        cell_grid[i][j].enabled = 0;
                    else
                        cell_grid[i][j].enabled = 1;
                }
            }
        }
    }
}