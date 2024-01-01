#include "global.h"
#include "particle.h"
#include "sprite.h"
#include "screen.h"

int32_t init_game();

int32_t window_width = 1280;
int32_t window_height = 720;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int32_t init_game()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow("Sand", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
	{
		printf("Could not initialize window: %s\n", SDL_GetError());
		return -1;
	}

	SDL_SetWindowMinimumSize(window, 800, 600);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL)
	{
		printf("Could not initialize renderer: %s\n", SDL_GetError());
		return -1;
	}

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		printf("Could not initialize SDL_Image: %s\n", IMG_GetError());
		return -1;
	}

	if (TTF_Init() < 0)
	{
		printf("Could not initialize SDL_ttf: %s\n", TTF_GetError());
		return -1;
	}

	return 0;
}