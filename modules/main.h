#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
bool Initialize (
    char* wTitle, int WINDOW_WIDTH, int WINDOW_HEIGHT,
    SDL_Window **gWindow, SDL_Renderer **gRenderer
  ) {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	if (TTF_Init() < 0) {
		printf("Error while initializing ttf.h\n");
		return false;
	}

	*gWindow = SDL_CreateWindow(
		wTitle,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED; // This is the flag for hardware acceleration.
	*gRenderer = SDL_CreateRenderer(*gWindow, -1, renderFlags); // Create a renderer for the window.

	if (!*gRenderer) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(*gWindow);
		SDL_Quit();
		return false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
	
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048) == -1 )
    {
        return false;    
    }
	return true;
}
