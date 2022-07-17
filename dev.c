#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "modules/structs.h"
#include "modules/CollidingBall.h"

const unsigned short WINDOW_WIDTH = 640;
const unsigned short WINDOW_HEIGHT = 480;
const unsigned short SPEED = 300;


int main (void) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *gameWindow = SDL_CreateWindow(
    "WASD & Arrows",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT, 0
  );

  Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
  SDL_Renderer *gameRenderer = SDL_CreateRenderer(gameWindow, -1, renderFlags); 

  if (!gameRenderer) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();
    return 1;
  }
  SDL_Surface *bgSurface = IMG_Load("assets/sprites/breakoutFondo.png");

  SDL_Surface *baseSurface = IMG_Load("assets/dude.png");
  if (!baseSurface) {
    printf("Error[-1]: %s\n", IMG_GetError());
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();
    return 1;
  }

  SDL_Surface *ballSurface = IMG_Load("assets/sprites/breakoutSprites.png");

  SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(gameRenderer, bgSurface);
  SDL_Texture *baseTexture = SDL_CreateTextureFromSurface(gameRenderer, baseSurface);
  SDL_Texture *ballTexture = SDL_CreateTextureFromSurface(gameRenderer, ballSurface);

  SDL_FreeSurface(baseSurface);
  SDL_FreeSurface(bgSurface);
  SDL_Rect backgroundRectangle = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

  if(!bgTexture){
    printf("Error: %s\n", SDL_GetError());
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();
    return 1;
  }
  if (!baseTexture) {
    printf("Error: %s\n", SDL_GetError());
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();
    return 1;
  }

  SDL_Rect spriteRectangle; 
  SDL_QueryTexture( 
    baseTexture,
    NULL, NULL,
    &spriteRectangle.w,
    &spriteRectangle.h
  );

  Ball *b = malloc(sizeof(Ball));
  initBall(b, WINDOW_WIDTH, WINDOW_HEIGHT);

  spriteRectangle.w /= 4;
  spriteRectangle.h /= 4;

  float xPosition = WINDOW_WIDTH / 2 - spriteRectangle.w / 2;
  float yPosition = WINDOW_HEIGHT / 2 - spriteRectangle.h / 2;
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

    spriteRectangle.x = (int)xPosition;
    spriteRectangle.y = (int)yPosition;


    // Keep player sprite in the bounds of the window
    if (xPosition <= 0)
      xPosition = 0;
    if (yPosition <= 0)
      yPosition = 0;
    if (xPosition >= (WINDOW_WIDTH - spriteRectangle.w))
      xPosition = WINDOW_WIDTH - spriteRectangle.w;
    if (yPosition >= (WINDOW_HEIGHT - spriteRectangle.h))
      yPosition = WINDOW_HEIGHT - spriteRectangle.h;
    updateBalls(b, 1, &closeWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    SDL_RenderClear(gameRenderer);
    SDL_RenderCopy(gameRenderer, bgTexture, NULL, &backgroundRectangle);
    SDL_RenderCopy(gameRenderer, baseTexture, NULL, &spriteRectangle);
    renderBall(*b, gameRenderer, ballTexture);
    SDL_RenderPresent(gameRenderer);

    SDL_Delay(1000 / 60);
  }

  SDL_DestroyTexture(baseTexture);
  SDL_DestroyRenderer(gameRenderer);
  SDL_DestroyWindow(gameWindow);
  SDL_Quit();

  return 0;
}
