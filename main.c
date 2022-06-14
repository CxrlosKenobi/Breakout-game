#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const double FPS = 60;
const int time = 3;

int main () {
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) >= 0) { // main logic of the game
		window = SDL_CreateWindow("Our first game test :D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window != NULL) {
			screenSurface = SDL_GetWindowSurface(window);
			SDL_UpdateWindowSurface(window);
			SDL_Delay(2000);
		} else
			printf("The window could not be created correctly :/\nSDL_ERROR: %s\n", SDL_GetError());
	} else
		printf("The Video could not run correctly\nSDL_ERROR: %s\n", SDL_GetError());

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
