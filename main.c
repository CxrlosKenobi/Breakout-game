#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
//
#include "modules/main.h"
#include "modules/structs.h"
#include "modules/Paddle/init.h"
#include "utils/utils.h"

/* Global Settings */
#define FPS (60)
#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (480)
#define SPEED (300)

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

int main () {
	bool start = Initialize("Breakout!", WINDOW_WIDTH, WINDOW_HEIGHT, &gWindow, &gRenderer);
	if (!start) return 1;

  Paddle paddle;
  bool summon_paddle = InitPaddle(
    &paddle,
    &gRenderer,
    &paddle.surface,
    &paddle.texture
  );
  if (!summon_paddle) return 1;

  // Place the paddle in the center of the screen
  paddle.xPos = WINDOW_WIDTH / 2 - paddle.rect.w / 2;
  paddle.yPos = WINDOW_HEIGHT / 2 - paddle.rect.h / 2;

  paddle.speed.x = 0; paddle.speed.y = 0;
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

    paddle.speed.x = 0; paddle.speed.y = 0; 
    if (up == 1 && down == 0)
      paddle.speed.y = -SPEED;
    if (down == 1 && up == 0)
      paddle.speed.y = SPEED;
    if (left == 1 && right == 0)
      paddle.speed.x = -SPEED;
    if (right == 1 && left == 0)
      paddle.speed.x = SPEED;

    paddle.xPos += paddle.speed.x / 60;
    paddle.yPos += paddle.speed.y / 60;

    paddle.rect.x = (int)paddle.xPos;
    paddle.rect.y = (int)paddle.yPos;

    // Keep player sprite in the bounds of the window
    if (paddle.xPos <= 0)
      paddle.xPos = 0;
    if (paddle.yPos <= 0)
      paddle.yPos = 0;
    if (paddle.xPos >= (WINDOW_WIDTH - paddle.rect.w))
      paddle.xPos = WINDOW_WIDTH - paddle.rect.w;
    if (paddle.yPos >= (WINDOW_HEIGHT - paddle.rect.h))
      paddle.yPos = WINDOW_HEIGHT - paddle.rect.h;
    
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, paddle.texture, NULL, &paddle.rect);
    SDL_RenderPresent(gRenderer);

    SDL_Delay(1000 / FPS);
  }

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
	
	return 0;
}

