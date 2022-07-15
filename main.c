#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
//
#include "modules/main.h"
#include "utils/utils.h"


/* Global variables */
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Surface *gHelloWorld = NULL;
SDL_Surface *gScreenSurface = NULL;

SDL_Surface* loadSurface (char* path) {
	SDL_Surface *loadedSurface = SDL_LoadBMP(path);
	if (loadedSurface == NULL)
		printf("Unable to load image from %s\nSDL_Error: %s\n", path, SDL_GetError());
	return loadedSurface;
}

bool loadMedia () {
	bool success = true;
	/* gHelloWorld = SDL_LoadBMP("assets/bird.bmp"); */
	/* if (gHelloWorld == NULL) { */
	/* 	printf("Unable to load owl image :/\nSDL_Error: %s\n", SDL_GetError()); */
	/* 	success = false; */
	/* } */
	return success;
}

int main () {
	initialize("Breakout!", WINDOW_WIDTH, WINDOW_HEIGHT, gWindow, gRenderer);


	destroyAll(gWindow, gRenderer, gHelloWorld);
	return 0;
}

