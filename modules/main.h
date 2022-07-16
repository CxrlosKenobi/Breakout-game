#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

bool Initialize (
    char* wTitle, int WINDOW_WIDTH, int WINDOW_HEIGHT,
    SDL_Window **gWindow, SDL_Renderer **gRenderer
  ) {
	
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
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

	return true;
}

// SDL_Texture *loadTexture (SDL_Renderer *gRenderer, char *path) {
// 	SDL_Texture *newTexture = NULL;
// 	SDL_Surface *loadedSurface = IMG_Load(path);
// 	if (loadedSurface == NULL) {
// 		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
// 	} else {
// 		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
// 		if (newTexture == NULL) {
// 			printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
// 		}
// 		SDL_FreeSurface(loadedSurface);
// 	}

// 	return newTexture;
// }

// bool loadMedia(SDL_Renderer *gRenderer, SDL_Texture *gTexture) {
// 	gTexture = loadTexture(gRenderer, 'assets/dude.png');
// 	if (gTexture == NULL) {
// 		printf("Failed to load texture image!\n");
// 		return false;
// 	}

// 	return true;
// }

// void close() {
// 	SDL_DestroyTexture(gTexture);
// 	gTexture = NULL;

// 	SDL_DestroyRenderer(gRenderer);
// 	gRenderer = NULL;

// 	SDL_DestroyWindow(gWindow);
// 	gWindow = NULL;

// 	IMG_Quit();
// 	SDL_Quit();
// }
