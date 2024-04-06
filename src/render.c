#include "global.h"
#include "render.h"
#include "sprite.h"
#include "particle.h"

static void render_text(state_t* state, const char* text, int32_t x, int32_t y, SDL_Color color);

char* str_advanced_options = "Advanced options:\n"
                             "\n"
                             "[F1]: toggle this menu\n"
                             "[F2]: pause/unpause game\n"
                             "[F3]: show/hide FPS\n"
                             "[F4]: toggle FPS limit\n"
                             "[F5]: show particle count\n"
                             "[F6]: highlight static particles\n"
                             "[up/down]: increase/decrease cursor size";

const char* material_text[] = { "sand", "water", "acid", "virus", "wood", "metal", "eraser" };

void render(state_t* state)
{
    *state->buttons[SAND] = (button_t){ .selected = (state->options.selected_material == SAND), .rect = (SDL_Rect){state->window_rect.w - 48, 0, 48, 48 } };
    *state->buttons[WATER] = (button_t){ .selected = (state->options.selected_material == WATER), .rect = (SDL_Rect){state->window_rect.w - 48, 48, 48, 48 } };
    *state->buttons[ACID] = (button_t){ .selected = (state->options.selected_material == ACID), .rect = (SDL_Rect){state->window_rect.w - 48, 96, 48, 48 } };
    *state->buttons[VIRUS] = (button_t){ .selected = (state->options.selected_material == VIRUS), .rect = (SDL_Rect){state->window_rect.w - 48, 144, 48, 48 } };
    *state->buttons[WOOD] = (button_t){ .selected = (state->options.selected_material == WOOD), .rect = (SDL_Rect){state->window_rect.w - 48, 192, 48, 48 } };
    *state->buttons[METAL] = (button_t){ .selected = (state->options.selected_material == METAL), .rect = (SDL_Rect){state->window_rect.w - 48, 240, 48, 48 } };
    *state->buttons[ERASER] = (button_t){ .selected = (state->options.selected_material == ERASER), .rect = (SDL_Rect){state->window_rect.w - 48, state->window_rect.h - 96, 48, 48 } };
    *state->buttons[RESET] = (button_t){ .selected = 0, .rect = (SDL_Rect){ state->window_rect.w - 48, state->window_rect.h - 48, 48, 48 } };

    SDL_Surface* grid_pixels = NULL;
    SDL_LockTextureToSurface(state->grid.tex, NULL, &grid_pixels);

    SDL_FillRect(grid_pixels, NULL, SDL_MapRGB(grid_pixels->format, 0x00, 0x00, 0x00));

    for (int i = 0; i < state->grid.rect.w; i++) {
        for (int j = 0; j < state->grid.rect.h; j++) {
            if (state->grid.cells[i][j] != NULL) {
                SDL_FillRect(
                    grid_pixels,
                    &(SDL_Rect) {i, j, 1, 1},
                    (state->options.show_static_particles && state->grid.cells[i][j]->is_static) ?
                    SDL_MapRGB(grid_pixels->format, 0xFF, 0x00, 0x00) :
                    SDL_MapRGB(
                            grid_pixels->format,
                            state->grid.cells[i][j]->material.color.r,
                            state->grid.cells[i][j]->material.color.g,
                            state->grid.cells[i][j]->material.color.b
                    )
                );
            }
        }
    }

    SDL_UnlockTexture(state->grid.tex);

    SDL_SetRenderDrawColor(state->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(state->renderer);

    SDL_RenderCopy(state->renderer, state->grid.tex, NULL, &(SDL_Rect){ 0, 0, state->grid.rect.w * SCALE, state->grid.rect.h * SCALE});

    SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(
            state->renderer,
            &(SDL_Rect) {
                    state->mouse_pos.x - (state->options.cursor_size - 1),
                    state->mouse_pos.y - (state->options.cursor_size - 1),
                    state->options.cursor_size * SCALE,
                    state->options.cursor_size * SCALE
            }
    );
    SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    for (int32_t i = 0; i < 8; i++) {
        SDL_RenderCopy(
                state->renderer,
                state->sprites.icon_atlas,
                &(SDL_Rect){ i * 48, (state->buttons[i]->selected == true) ? 48 : 0, 48, 48 },
                &state->buttons[i]->rect
        );
    }

    SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(state->renderer, &(SDL_Rect){ state->window_rect.w - 48, 288, 47, state->window_rect.h - 384});
    SDL_SetRenderDrawColor(state->renderer, 0x00, 0x00, 0x00, 0xFF);

    render_text(state, material_text[state->options.selected_material], 10, state->window_rect.h - 30, white);

    render_text(state, "1", state->window_rect.w - 64, 14, white);
    render_text(state, "2", state->window_rect.w - 64, 62, white);
    render_text(state, "3", state->window_rect.w - 64, 110, white);
    render_text(state, "4", state->window_rect.w - 64, 158, white);
    render_text(state, "5", state->window_rect.w - 64, 206, white);
    render_text(state, "6", state->window_rect.w - 64, 254, white);
    render_text(state, "0", state->window_rect.w - 64, state->window_rect.h - 82, white);
    render_text(state, "c", state->window_rect.w - 64, state->window_rect.h - 34, white);

    if (state->options.show_advanced_options) {
        render_text(state, str_advanced_options, 50, 50, white);
    }

    if (state->options.show_fps) {
        char str_fps[64];
        snprintf(str_fps, 64, "fps: %u", state->time.fps);
        render_text(state, str_fps, 10, 10, (state->time.fps >= 120) ? white : (SDL_Color){ 0xFF, 0xBF, 0x00, 0xFF });
    }

    SDL_RenderPresent(state->renderer);
}

void init_buttons(state_t* state)
{
    for (int32_t i = 0; i < 8; i++) {
        state->buttons[i] = malloc(sizeof(button_t));
    }
}

static void render_text(state_t* state, const char* text, int32_t x, int32_t y, SDL_Color color)
{
    int32_t w, h, left = x;
    TTF_SizeText(state->font.ttf, "a", &w, &h);
    SDL_SetTextureColorMod(state->font.atlas, color.r, color.g, color.b);
    for (int32_t i = 0; i < strlen(text); i++) {
        SDL_Rect *char_glyph = &state->font.glyphs[text[i]];
        if (text[i] == '\n') {
            x = left;
            y += h;
        }
        else {
            SDL_RenderCopy(state->renderer, state->font.atlas, char_glyph, &(SDL_Rect){ x, y, char_glyph->w, char_glyph->h });
            x += w;
        }
    }
}