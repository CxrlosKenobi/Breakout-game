#include <stdlib.h>
#include <SDL2/SDL.h>

bool initialize (
    char* wTitle, int WINDOW_WIDTH, int WINDOW_HEIGHT,
    SDL_Window *gWindow, SDL_Renderer *gRenderer
  ) {
	
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	gWindow = SDL_CreateWindow(
		wTitle,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED; // This is the flag for hardware acceleration.
	gRenderer = SDL_CreateRenderer(gWindow, -1, renderFlags); // Create a renderer for the window.
	if (!gRenderer) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		return false;
	}
}
