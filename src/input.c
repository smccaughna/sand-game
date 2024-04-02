#include "global.h"
#include "input.h"
#include "particle.h"
#include "render.h"

void handle_input(state_t* state)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                state->should_quit = true;
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    update_grid_size(state);
                }
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_0: state->selected_material = ERASER; break;
                    case SDLK_1: state->selected_material = SAND; break;
                    case SDLK_2: state->selected_material = WATER; break;
                    case SDLK_3: state->selected_material = ACID; break;
                    case SDLK_4: state->selected_material = VIRUS; break;
                    case SDLK_5: state->selected_material = WOOD; break;
                    case SDLK_6: state->selected_material = METAL; break;
                    case SDLK_c: clear_grid(state); break;
                    case SDLK_F1: state->options.show_advanced_options = !state->options.show_advanced_options; break;
                    case SDLK_F2: state->options.pause_game = !state->options.pause_game; break;
                    case SDLK_F3: state->options.show_fps = !state->options.show_fps; break;
                    case SDLK_F4: state->options.limit_fps = !state->options.limit_fps; break;
                    case SDLK_F5: state->options.show_particle_count = !state->options.show_particle_count; break;
                    case SDLK_F6: state->options.show_static_particles = !state->options.show_static_particles; break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                for (int i = 0; i < 8; i++) {
                    if (SDL_PointInRect(&(SDL_Point){ e.button.x, e.button.y }, &state->buttons[i]->rect)) {
                        if (i == RESET) {
                            clear_grid(state);
                        } else {
                            state->selected_material = (int8_t)i;
                        }
                    }
                }
                break;
        }
    }

    int32_t x, y;
    if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(1))
    {
        SDL_Color current_color;

        switch (state->selected_material) {
            case SAND:  current_color = color_sand; break;
            case WATER: current_color = color_water; break;
            case ACID:  current_color = color_acid; break;
            case VIRUS: current_color = color_virus; break;
            case WOOD:  current_color = color_wood; break;
            case METAL: current_color = color_metal; break;
        }

        int32_t grid_x = x / SCALE;
        int32_t grid_y = y / SCALE;

        if (state->selected_material == ERASER) {
            if (!check_out_of_bounds(&state->grid.rect, grid_x, grid_y) && state->grid.cells[grid_x][grid_y] != NULL) {
                activate_neighbors(state, grid_x, grid_y);
                free(state->grid.cells[grid_x][grid_y]);
                state->grid.cells[grid_x][grid_y] = NULL;
            }
        }
        else {
            if (!check_out_of_bounds(&state->grid.rect, grid_x, grid_y) && state->grid.cells[grid_x][grid_y] == NULL) {
                state->grid.cells[grid_x][grid_y] = malloc(sizeof(particle_t));
                *state->grid.cells[grid_x][grid_y] = (particle_t) {
                    {
                        state->selected_material,
                        current_color
                    },
                    false,
                    state->time.tick - 1
                };
            }
        }
    }
}