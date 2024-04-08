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
                    case SDLK_0: state->options.selected_material = ERASER; break;
                    case SDLK_1: state->options.selected_material = SAND; break;
                    case SDLK_2: state->options.selected_material = WATER; break;
                    case SDLK_3: state->options.selected_material = ACID; break;
                    case SDLK_4: state->options.selected_material = VIRUS; break;
                    case SDLK_5: state->options.selected_material = WOOD; break;
                    case SDLK_6: state->options.selected_material = METAL; break;
                    case SDLK_c: clear_grid(state); break;
                    case SDLK_F1: state->options.show_advanced_options = !state->options.show_advanced_options; break;
                    case SDLK_F2: state->options.pause_game = !state->options.pause_game; break;
                    case SDLK_F3: state->options.show_fps = !state->options.show_fps; break;
                    case SDLK_F4: state->options.limit_fps = !state->options.limit_fps; break;
                    case SDLK_F5: state->options.show_particle_count = !state->options.show_particle_count; break;
                    case SDLK_F6: state->options.show_static_particles = !state->options.show_static_particles; break;
                    case SDLK_UP: state->options.cursor_size += (state->options.cursor_size < 15) ? 2 : 0; break;
                    case SDLK_DOWN: state->options.cursor_size -= (state->options.cursor_size > 1) ? 2 : 0; break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                for (int i = 0; i < 8; i++) {
                    if (SDL_PointInRect(&(SDL_Point){ e.button.x, e.button.y }, &state->buttons[i]->rect)) {
                        if (i == RESET) {
                            clear_grid(state);
                        } else {
                            state->options.selected_material = (int8_t)i;
                        }
                    }
                }
                break;
        }
    }

    if (SDL_GetMouseState(&state->mouse_pos.x, &state->mouse_pos.y) & SDL_BUTTON(1))
    {
        SDL_Color current_color;

        switch (state->options.selected_material) {
            case SAND:  current_color = color_sand; break;
            case WATER: current_color = color_water; break;
            case ACID:  current_color = color_acid; break;
            case VIRUS: current_color = color_virus; break;
            case WOOD:  current_color = color_wood; break;
            case METAL: current_color = color_metal; break;
        }

        int32_t x = state->mouse_pos.x / SCALE;
        int32_t y = state->mouse_pos.y / SCALE;

        if (state->options.selected_material == ERASER) {
            for (int32_t i = -(state->options.cursor_size - 1) / 2; i <= (state->options.cursor_size - 1) / 2; i++) {
                for (int32_t j = -(state->options.cursor_size - 1) / 2; j <= (state->options.cursor_size - 1) / 2; j++) {
                    if (!check_out_of_bounds(&state->grid.rect, x + i, y + j)) {
                        if (state->grid.cells[x + i][y + j] != NULL) {
                            activate_neighbors(state, x + i, y + j);
                            free(state->grid.cells[x + i][y + j]);
                            state->grid.cells[x + i][y + j] = NULL;
                        }
                    }
                }
            }
        }
        else {
            if (!check_out_of_bounds(&state->grid.rect, x, y)) {
                for (int32_t i = -(state->options.cursor_size - 1) / 2; i <= (state->options.cursor_size - 1) / 2; i++) {
                    for (int32_t j = -(state->options.cursor_size - 1) / 2; j <= (state->options.cursor_size - 1) / 2; j++) {
                        if (!check_out_of_bounds(&state->grid.rect, x + i, y + j)) {
                            if (state->grid.cells[x + i][y + j] == NULL) {
                                state->grid.cells[x + i][y + j] = malloc(sizeof(particle_t));
                            }
                            *state->grid.cells[x + i][y + j] = (particle_t) {
                                    {
                                            state->options.selected_material,
                                            current_color
                                    },
                                    false,
                                    state->time.tick - 1
                            };
                        }
                    }
                }
            }
        }
    }
}