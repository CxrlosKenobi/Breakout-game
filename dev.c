#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//
#include "modules/structs.h"
#include "utils/utils.h"
#include "modules/CollidingBall"

const unsigned short WINDOW_WIDTH = 640;
const unsigned short WINDOW_HEIGHT = 480;
const unsigned short SPEED = 300;

int main (void) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *gWindow = SDL_CreateWindow(
    "WASD & Arrows",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT, 0
  );

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED; // This is the flag for hardware acceleration.
	SDL_Renderer *gRenderer = SDL_CreateRenderer(gWindow, -1, renderFlags); // Create a renderer for the window.
	
	if (!gRenderer) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		return 1;
	}

	SDL_Surface *bgSurface = IMG_Load("assets/sprites/breakoutFondo.png");
	SDL_Surface *baseSurface = IMG_Load("assets/dude.png");
	if (!baseSurface) {
		printf("Error[base]: %s\n", IMG_GetError());
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		return 1;
	}

	SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(gRenderer, bgSurface);
	SDL_Texture *baseTexture = SDL_CreateTextureFromSurface(gRenderer, baseSurface);

	SDL_FreeSurface(baseSurface);
	SDL_FreeSurface(bgSurface);
	SDL_Rect bgRectangle = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	if (!bgTexture) {
		printf("Error [bgTexture]: %s\n", SDL_GetError());
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		return 1;
	}

	if (!baseTexture) {
		printf("Error [baseTexture]: %s\n", SDL_GetError());
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		return 1;
	}

	SDL_Rect baseRectangle;
	SDL_QueryTexture(
		baseTexture,
		NULL, NULL,
		&baseRectangle.w,
		&baseRectangle.h
	);

	baseRectangle.w /= 4;
	baseRectangle.h /= 4;

	float xPosition = WINDOW_WIDTH / 2 - baseRectangle.w / 2;
	float yPosition = WINDOW_HEIGHT / 2 - baseRectangle.h / 2;
	float xSpeed = 0, ySpeed = 0;

	int up = 0, down = 0, left = 0, right = 0;
	bool closeWindow = 0;
	while (closeWindow == 0) {
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent)) {
			switch (gameEvent.type) {
				case SDL_QUIT:
					closeWindow = 1;
					break;
				
				case SDL_KEYDOWN:
					switch (gameEvent.key.keysym.scancode) {
						case SDL_SCANCODE_UP:
						case SDL_SCANCODE_W:
							up = 1;
							break;
						case SDL_SCANCODE_DOWN:
						case SDL_SCANCODE_S:
							down = 1;
							break;
						case SDL_SCANCODE_LEFT:
						case SDL_SCANCODE_A:
							left = 1;
							break;
						case SDL_SCANCODE_RIGHT:
						case SDL_SCANCODE_D:
							right = 1;
							break;
						default:
							break;
					}
					break;

				case SDL_KEYUP:
					switch (gameEvent.key.keysym.scancode) {
						case SDL_SCANCODE_UP:
						case SDL_SCANCODE_W:
							up = 0;
							break;
						case SDL_SCANCODE_DOWN:
						case SDL_SCANCODE_S:
							down = 0;
							break;
						case SDL_SCANCODE_LEFT:
						case SDL_SCANCODE_A:
							left = 0;
							break;
						case SDL_SCANCODE_RIGHT:
						case SDL_SCANCODE_D:
							right = 0;
							break;
						default:
							break;
					}
					break;
				
				default:
					break;
			}
		}

		xSpeed = 0; ySpeed = 0;
		if (up == 1 && down == 0)
			ySpeed = -SPEED;
		if (down == 1 && up == 0)
			ySpeed = SPEED;
		if (left == 1 && right == 0)
			xSpeed = -SPEED;
		if (right == 1 && left == 0)
			xSpeed = SPEED;
		
		xPosition += xSpeed / 60;
		yPosition += ySpeed / 60;

		baseRectangle.x = (int)xPosition;
		baseRectangle.y = (int)yPosition;

		// Keep player sprite in the bounds of the window
		if (xPosition <= 0)
			xPosition = 0;
		if (yPosition <= 0)
			yPosition = 0;
		if (xPosition >= (WINDOW_WIDTH - baseRectangle.w))
			xPosition = WINDOW_WIDTH - baseRectangle.w;
		if (yPosition >= (WINDOW_HEIGHT - baseRectangle.h))
			yPosition = WINDOW_HEIGHT - baseRectangle.h;

		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, bgTexture, NULL, &bgRectangle);
		SDL_RenderCopy(gRenderer, baseTexture, NULL, &baseRectangle);
		SDL_RenderPresent(gRenderer);

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
	
	return 0;
}
