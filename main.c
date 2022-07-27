#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//
#include "modules/main.h"
#include "modules/structs.h"
#include "modules/Paddle/init.h"
#include "modules/CollidingBall/CollidingBall.h"
#include "modules/Bricks/Bricks.h"
#include "utils/utils.h"
#include "views/menu.h"
#include "views/credits.h"

/* Global Settings */
#define FPS (60)
#define SPEED (300)


// Leave'em as this type of const pls, need it for ball calculating collides with walls
const unsigned short WINDOW_WIDTH = 640;
const unsigned short WINDOW_HEIGHT = 480;

enum menu_option {menu, game, highscores, credits, quit};


SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

int main() {
  srand(time(NULL));
  bool start = Initialize("Breakout!", WINDOW_WIDTH, WINDOW_HEIGHT, &gWindow, &gRenderer);
  if (!start)
    return 1;

  SDL_Surface *bgSurface = IMG_Load("assets/sprites/scenes/fondoOscuro.png");
  SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(gRenderer, bgSurface);
  SDL_Surface *menuBgSurface = IMG_Load("assets/sprites/scenes/breakoutFondo.png");
  SDL_Texture *menuBgTexture = SDL_CreateTextureFromSurface(gRenderer, menuBgSurface);
  TTF_Font* minecraftFont = NULL;
  minecraftFont = TTF_OpenFont("assets/fonts/Minecraft.ttf", 28);
  if (minecraftFont == NULL)
    printf("An error has occured while loading minecraft font\nSDL_Error: %s\n", SDL_GetError());

  // Bricks setup
  const unsigned short rows = 6;
  const unsigned short cols = 10;
  Brick **bricks = createRandomBrickMatrix(rows, cols);
  SDL_Surface *brickSurface[4];
  brickSurface[0] =  NULL;
  brickSurface[1] =  IMG_Load("assets/sprites/bricks/bloqueNaranja.png");
  brickSurface[2] =  IMG_Load("assets/sprites/bricks/bloqueVerde.png");
  brickSurface[3] =  IMG_Load("assets/sprites/bricks/bloqueAzul.png");
  SDL_Texture *brickTextures[4];
  brickTextures[1] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[1]);
  brickTextures[2] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[2]);
  brickTextures[3] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[3]);

  // Ball setup
  Ball *b = malloc(sizeof(Ball));
  unsigned short ballsAmount = 1;
  initBall(b, WINDOW_WIDTH, WINDOW_HEIGHT);
  SDL_Surface *ballSurface = IMG_Load("assets/sprites/ball.png");
  SDL_Texture *ballTexture = SDL_CreateTextureFromSurface(gRenderer, ballSurface);

  // Paddle setup
  Paddle paddle;
  bool summon_paddle = initPaddle(
      &paddle,
      &gRenderer,
      &paddle.surface,
      &paddle.texture);
  if (!summon_paddle)
    return 1;

  // Place the paddle in the center of the screen
  paddle.xPos = WINDOW_WIDTH / 2 - paddle.rect.w / 2;
  paddle.yPos = WINDOW_HEIGHT / 2 - paddle.rect.h / 2;

  paddle.speed.x = 0;
  paddle.speed.y = 0;
  int up = 0, down = 0, left = 0, right = 0;
  bool closeWindow = 0;
  bool pause = true;
  unsigned short view = menu;
  unsigned short hoveredOption = game;
  SDL_Event gameEvent;
  while (!closeWindow) {
    switch (view) {
      case menu:
        while (SDL_PollEvent(&gameEvent)) {
          switch (gameEvent.type) {
            case SDL_QUIT:
              closeWindow = true;
              break;

            case SDL_KEYUP:
              switch (gameEvent.key.keysym.scancode) {
                case SDL_SCANCODE_DOWN:
                  if (hoveredOption == quit) hoveredOption = game;
                  else hoveredOption++;
                  break;
                case SDL_SCANCODE_UP:
                  if (hoveredOption == game) hoveredOption = quit;
                  else hoveredOption--;
                  break;
                case SDL_SCANCODE_RETURN:
                  view = hoveredOption;
                  break;
              }
              break;
          }
        }
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, menuBgTexture, NULL, NULL);
        renderMenu(hoveredOption, gRenderer, minecraftFont);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000 / FPS);

        break;
      case game:
        SDL_Event gameEvent;
        while (SDL_PollEvent(&gameEvent)) {
          switch (gameEvent.type) {
            case SDL_QUIT:
              closeWindow = true;
              break;

            case SDL_KEYDOWN:
              switch (gameEvent.key.keysym.scancode) {
                case SDL_SCANCODE_SPACE:
                  pause = !pause;
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
        if (!pause) {
          paddle.speed.x = 0;
          paddle.speed.y = 0;
          if (up == 1 && down == 0)
            paddle.speed.y = -SPEED;
          if (down == 1 && up == 0)
            paddle.speed.y = SPEED;
          if (left == 1 && right == 0)
            paddle.speed.x = -SPEED;
          if (right == 1 && left == 0)
            paddle.speed.x = SPEED;

          paddle.xPos += paddle.speed.x / 60;
          // paddle.yPos += paddle.speed.y / 60;
          paddle.yPos = 400;

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

          // updateBalls(b, 1, &closeWindow, WINDOW_WIDTH, WINDOW_HEIGHT, paddle);
          // Update Balls state and calculate collisions
          for (unsigned short i=0;i<ballsAmount;++i) {
            manageWallCollision(b+i, &closeWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
            managePaddleCollision(b+i, paddle);
            manageBricksCollision(bricks, b+i, WINDOW_WIDTH, 2*WINDOW_HEIGHT/5, rows, cols, 0, 0);
            (b+i)->pos.x += (b+i)->vel.x;
            (b+i)->pos.y += (b+i)->vel.y;
          }
        }
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);
        SDL_RenderCopy(gRenderer, paddle.texture, NULL, &paddle.rect);
        // Draw paddle rect
        // SDL_RenderFillRect(gRenderer, &paddle.rect);
        renderBricks(bricks, gRenderer, brickTextures, WINDOW_WIDTH, 2*WINDOW_HEIGHT/5, rows, cols, 0, 0);
        renderBall(*b, gRenderer, ballTexture);
        // renderBallSquare(*b, gRenderer);

        SDL_RenderPresent(gRenderer);

        SDL_Delay(1000 / FPS);
        break;
      case highscores:
        while (SDL_PollEvent(&gameEvent)) {
          switch (gameEvent.type) {
            case SDL_QUIT:
              closeWindow = true;
              break;
          }
        }
        break;
      case credits:
        while (SDL_PollEvent(&gameEvent)) {
          switch (gameEvent.type) {
            case SDL_QUIT:
              closeWindow = true;
              break;
            case SDL_KEYDOWN:
              switch (gameEvent.key.keysym.scancode) {
                case SDL_SCANCODE_Q:
                  view = menu;
                  break;
              }
          }
        }
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, menuBgTexture, NULL, NULL);
        renderCredits(gRenderer, minecraftFont);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000 / FPS);
        break;
      case quit:
        closeWindow = true;
        break;
    }
  }
  freeBricks(bricks, rows);
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();

  return 0;
}