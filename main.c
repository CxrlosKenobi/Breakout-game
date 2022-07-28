#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
//
#include "modules/main.h"
#include "modules/structs.h"
#include "modules/Paddle/init.h"
#include "modules/CollidingBall/CollidingBall.h"
#include "modules/Bricks/Bricks.h"
#include "utils/utils.h"

/* Global Settings */
#define FPS (60)
#define SPEED (300)


// Leave'em as this type of const pls, need it for ball calculating collides with walls
const unsigned short WINDOW_WIDTH = 640;
const unsigned short WINDOW_HEIGHT = 480;


SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

int main() {
  srand(time(NULL));
  bool start = Initialize("Breakout!", WINDOW_WIDTH, WINDOW_HEIGHT, &gWindow, &gRenderer);
  if (!start)
    return 1;

  SDL_Surface *bgSurface = IMG_Load("assets/sprites/fondoOscuro.png");
  SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(gRenderer, bgSurface);

  // Bricks setup
  const unsigned short rows = 6;
  const unsigned short cols = 10;
  const unsigned short MARGINX = 0;
  const unsigned short MARGINY = 0;
  Brick **bricks = createRandomBrickMatrix(rows, cols);
  SDL_Surface *brickSurface[4];
  brickSurface[0] =  NULL;
  brickSurface[1] =  IMG_Load("assets/sprites/bloqueNaranja.png");
  brickSurface[2] =  IMG_Load("assets/sprites/bloqueVerde.png");
  brickSurface[3] =  IMG_Load("assets/sprites/bloqueAzul.png");
  SDL_Texture *brickTextures[4];
  brickTextures[1] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[1]);
  brickTextures[2] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[2]);
  brickTextures[3] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[3]);

  //Sounds setups
  // Take the sound and music from the structs.h file
  Mixers mixers;
  mixers.sound = Mix_LoadWAV("assets/sounds/bounce.wav");
  mixers.music = Mix_LoadMUS("assets/sounds/song.mp3");

  if(!mixers.music || !mixers.sound)
    printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());

  Mix_PlayMusic(mixers.music, -1);

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
  //Font setup
  SDL_Color textColor = {255, 255, 255};
  TTF_Font *font = NULL;
  font = TTF_OpenFont("assets/fonts/DePixelBreit.ttf", 30);
  SDL_Surface *PointsSurface = TTF_RenderText_Solid(font, "Puntos: 0",textColor );
  SDL_Texture *PointsTexture = SDL_CreateTextureFromSurface(gRenderer, PointsSurface);
  SDL_Rect PointsRect = {30, 30, PointsSurface->w, PointsSurface->h};
  TTF_CloseFont(font);
  SDL_FreeSurface(PointsSurface);
  // Place the paddle in the center of the screen
  paddle.xPos = WINDOW_WIDTH / 2 - paddle.rect.w / 2;
  paddle.yPos = WINDOW_HEIGHT / 2 - paddle.rect.h / 2;

  paddle.speed.x = 0;
  paddle.speed.y = 0;
  int up = 0, down = 0, left = 0, right = 0;
  bool closeWindow = 0;
  bool pause = 1;
  while (!closeWindow)
  {
    SDL_Event gameEvent;
    while (SDL_PollEvent(&gameEvent))
    {
      switch (gameEvent.type)
      {
      case SDL_QUIT:
        closeWindow = 1;
        break;

      case SDL_KEYDOWN:
        switch (gameEvent.key.keysym.scancode) {
          // case SDL_SCANCODE_SPACE:
          //   pause = !pause;
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
    for (unsigned short i=0;i<ballsAmount;++i) {
      manageWallCollision(b+i, &closeWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
      managePaddleCollision(b+i, paddle, mixers.sound);
      manageBricksCollision(bricks, b+i, WINDOW_WIDTH, 2*WINDOW_HEIGHT/5, rows, cols, MARGINX, MARGINY);
      (b+i)->pos.x += (b+i)->vel.x;
      (b+i)->pos.y += (b+i)->vel.y;
    }

    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);
    SDL_RenderCopy(gRenderer, paddle.texture, NULL, &paddle.rect);
    renderBricks(bricks, gRenderer, brickTextures, WINDOW_WIDTH, 2 * WINDOW_HEIGHT / 5, rows, cols, MARGINX, MARGINY);
    renderBall(*b, gRenderer, ballTexture);
    SDL_RenderCopy(gRenderer, PointsTexture, NULL, &PointsRect);
    SDL_RenderPresent(gRenderer);

    SDL_Delay(1000 / FPS);
  }
  freeBricks(bricks, rows);
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();

  return 0;
}
