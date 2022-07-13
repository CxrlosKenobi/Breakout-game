#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include "modules/BaseController.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

SDL_Surface* loadSurface (char* path) {
	SDL_Surface *loadedSurface = SDL_LoadBMP(path);
	if (loadedSurface == NULL)
		printf("Unable to load image from %s\nSDL_Error: %s\n", path, SDL_GetError());
	return loadedSurface;
}

bool init () { // Start up environment
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SLD couldn't initialize :/\nSDL_Error: %s\n", SDL_GetError());
		success = false;
	} else {
		gWindow = SDL_CreateWindow("Break Out!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window couldn't be created :/\nSDL_Error: %s\n", SDL_GetError());
			success = false;
		} else
			gScreenSurface = SDL_GetWindowSurface(gWindow);
	}
	return success;
}

bool loadMedia () {
	bool success = true;
	gHelloWorld = SDL_LoadBMP("assets/bird.bmp");
	if (gHelloWorld == NULL) {
		printf("Unable to load owl image :/\nSDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	return success;
}

void killSDL () { // Frees media and kill SDL
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int main (int argc, char* argv[]) {
	if (!init()) {
		printf("Failed to initialize :(\n");
	} else {
		if (!loadMedia()) {
			printf("Failed to load Media :(\n");
		} else {
			SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
			SDL_UpdateWindowSurface(gWindow);
			bool quit = false;
			SDL_Event e;
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = true;
				}
				SDL_Delay(100);
				/* SDL_BlitSurface(gXOut, NUll, gScreenSurface, NULL); */
				/* SDL_UpdateWindowSurface(gWindow); */
			}
		}
	}
	killSDL();
	return 0;
}