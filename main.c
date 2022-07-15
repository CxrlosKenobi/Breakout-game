#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//
#include "modules/main.h"
#include "utils/utils.h"


/* Global variables */
#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (480)
#define SPEED (300)

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

SDL_Surface *bgSurface = NULL;
SDL_Surface *baseSurface = NULL;
SDL_Texture *bgTexture = NULL;
SDL_Texture *baseTexture = NULL;

int main () {
	initialize("Breakout!", WINDOW_WIDTH, WINDOW_HEIGHT, gWindow, gRenderer);

	bgSurface = IMG_Load("assets/sprites/breakoutFondo.png");
	baseSurface = IMG_Load("assets/dude.png");

	if (!baseSurface) {
		printf("Error[base]: %s\n", IMG_GetError());
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		return 1;
	}

	bgTexture = SDL_CreateTextureFromSurface(gRenderer, bgSurface);
	baseTexture = SDL_CreateTextureFromSurface(gRenderer, baseSurface);

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
	int closeWindow = 0;
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

