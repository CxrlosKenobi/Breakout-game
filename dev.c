#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
#define FPS (50)
#define SPEED (300)


// Leave'em as this type of const pls, need it for ball calculating collides with walls
const unsigned short WINDOW_WIDTH = 640;
const unsigned short WINDOW_HEIGHT = 480;

enum menu_option {menu, game, highscores, credits, quit, mute};


typedef struct {
  unsigned val;
  char string[11];
  SDL_Rect rect;
  SDL_Color color;
}Score;

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
  SDL_Surface *menuTitleSurface = IMG_Load("assets/Images/title.png");
  SDL_Surface *heartSurface = IMG_Load("assets/sprites/minecraft_heart.png");
  SDL_Texture *heartTexture = SDL_CreateTextureFromSurface(gRenderer, heartSurface);
  SDL_Rect menuTitleRect;
  menuTitleRect.x = 65;
  menuTitleRect.y = 0;
  menuTitleRect.w = 500;
  menuTitleRect.h = 200;
  SDL_Texture *menuTitleTexture = SDL_CreateTextureFromSurface(gRenderer, menuTitleSurface);
  TTF_Font* minecraftFont = NULL;
  minecraftFont = TTF_OpenFont("assets/fonts/SF Atarian System Extended Bold Italic.ttf", 40);
  // minecraftFont = TTF_OpenFont("assets/fonts/ARCADECLASSIC.ttf", 40);
  if (minecraftFont == NULL)
    printf("An error has occured while loading minecraft font\nSDL_Error: %s\n", SDL_GetError());
  Score score;
  score.rect.x = 10;
  score.rect.y = 420;
  score.rect.w = 110;
  score.rect.h = 30;
  score.color.r = score.color.g = score.color.b = 0;
  unsigned lives = 3;

  // Bricks setup
  const unsigned short rows = 6;
  const unsigned short cols = 10;
  unsigned bricks_amount;
  // Brick **bricks = createRandomBrickMatrix(rows, cols, &bricks_amount);
  Brick **bricks;
  SDL_Surface *brickSurface[4];
  brickSurface[0] =  NULL;
  brickSurface[1] =  IMG_Load("assets/sprites/bricks/bloqueNaranja.png");
  brickSurface[2] =  IMG_Load("assets/sprites/bricks/bloqueVerde.png");
  brickSurface[3] =  IMG_Load("assets/sprites/bricks/bloqueAzul.png");
  SDL_Texture *brickTextures[4];
  brickTextures[1] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[1]);
  brickTextures[2] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[2]);
  brickTextures[3] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[3]);

  //Sounds and mixer setup
  Mix_Music *music = NULL;
  Mix_Chunk *bounce = NULL;
  Mix_Chunk *brickSound= NULL;
  Mix_Chunk *selectionSound = NULL;

  
  music = Mix_LoadMUS("assets/sounds/music.mp3");
  bounce = Mix_LoadWAV("assets/sounds/bounce.mp3");
  brickSound = Mix_LoadWAV("assets/sounds/brick.mp3");
  selectionSound = Mix_LoadWAV("assets/sounds/selectionMenu.mp3");
  if (music == NULL)
    printf("An error has occured while loading music\nSDL_Error: %s\n", SDL_GetError());
  if (bounce == NULL)
    printf("An error has occured while loading sound\nSDL_Error: %s\n", SDL_GetError());
  if (brickSound == NULL)
    printf("An error has occured while loading sound\nSDL_Error: %s\n", SDL_GetError());
  Sound sounds;
  sounds.bounce = brickSound;
  Mix_PlayMusic(music, -1);
  Mix_VolumeMusic(MIX_MAX_VOLUME/12);

  Mix_VolumeChunk(bounce, MIX_MAX_VOLUME/12);
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
  bool win = false;
  bool lose = true;
  bool frame = true;
  unsigned short view = menu;
  unsigned short hoveredOption = game;
  score.val = 0;
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
                  Mix_PlayChannel(-1, selectionSound, 0);
                  if (hoveredOption == mute) hoveredOption = game;
                  else hoveredOption++;
                  break;
                case SDL_SCANCODE_UP:
                  Mix_PlayChannel(-1, selectionSound, 0);
                  if (hoveredOption == game) hoveredOption = mute;
                  else hoveredOption--;
                  break;
                case SDL_SCANCODE_RETURN:
                  if(hoveredOption==mute)
                    Mix_VolumeMusic(0)==0?Mix_VolumeMusic(MIX_MAX_VOLUME/12):Mix_VolumeMusic(0);
                   else{
                  view = hoveredOption;
                  initBall(b, WINDOW_WIDTH, WINDOW_HEIGHT);
                  initPaddle(&paddle, &gRenderer, &paddle.surface, &paddle.texture);
                  paddle.xPos = WINDOW_WIDTH / 2 - paddle.rect.w / 2;
                  paddle.yPos = WINDOW_HEIGHT / 2 - paddle.rect.h / 2;
                  paddle.speed.x = 0;
                  paddle.speed.y = 0;
                  bricks = createRandomBrickMatrix(rows, cols, &bricks_amount);
                  up = down = left = right = 0;
                  pause = true;
                  score.val = 0;
                  win = false;
                  lose = false;
                  lives = 3;
                  frame = true;
                  break;
                  }
                }
              break;
          }
        }
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, menuBgTexture, NULL, NULL);
        SDL_RenderCopy(gRenderer, menuTitleTexture, NULL, &menuTitleRect);
        renderMenu(hoveredOption, gRenderer, minecraftFont);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000 / FPS);

        break;
      case game: ;
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
            paddle.speed.y = -SPEED/2;
          if (down == 1 && up == 0)
            paddle.speed.y = SPEED/2;
          if (left == 1 && right == 0)
            paddle.speed.x = -SPEED/2;
          if (right == 1 && left == 0)
            paddle.speed.x = SPEED/2;

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

          // Update Balls state and calculate collisions
          for (unsigned short i=0;i<ballsAmount;++i) {
            if (manageWallCollision(b+i, &view, WINDOW_WIDTH, WINDOW_HEIGHT, sounds.bounce)) {
              initBall(b, WINDOW_WIDTH, WINDOW_HEIGHT);
              initPaddle(&paddle, &gRenderer, &paddle.surface, &paddle.texture);
              paddle.xPos = WINDOW_WIDTH / 2 - paddle.rect.w / 2;
              paddle.yPos = WINDOW_HEIGHT / 2 - paddle.rect.h / 2;
              paddle.rect.x = paddle.xPos;
              paddle.speed.x = 0;
              paddle.speed.y = 0;
              up = down = left = right = 0;
              pause = true;
              frame = false;
              lives--;
              Mix_PlayChannel(-1,brickSound, 0);
            }

            if(managePaddleCollision(b+i, paddle))
              Mix_PlayChannel(-1, bounce, 0);
            if (manageBricksCollision(bricks, b+i, WINDOW_WIDTH, 2*WINDOW_HEIGHT/5, rows, cols, 0, 0, &score.val, &bricks_amount)) {
              Mix_PlayChannel(-1, brickSound, 0);
            }
            (b+i)->pos.x += ((b+i)->vel.x) / 2;
            (b+i)->pos.y += ((b+i)->vel.y) / 2;
          }
          if (!bricks_amount)
            win = true;
          if (!lives) {
            lose = true;
          }
          if (win || lose) {
            view = menu;
          }
          frame = !frame;
        }
        // printf("Velocity of ball in x: %f\n", b->vel.x);
        // printf("Velocity of ball in y: %f\n\n", b->vel.y);
        if (frame) {
          SDL_RenderClear(gRenderer);
          SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);
          SDL_RenderCopy(gRenderer, paddle.texture, NULL, &paddle.rect);
          // Draw paddle rect
          // SDL_RenderFillRect(gRenderer, &paddle.rect);
          renderBricks(bricks, gRenderer, brickTextures, WINDOW_WIDTH, 2*WINDOW_HEIGHT/5, rows, cols, 0, 0);
          renderBall(*b, gRenderer, ballTexture);
          sprintf(score.string, "score: %4d", score.val);
          // itoa(score.val, score.string, 10);
          renderText(score.string, score.rect, gRenderer, minecraftFont, score.color); // render Score
          renderLives(gRenderer, lives, heartTexture);
          // renderBallSquare(*b, gRenderer);

          SDL_RenderPresent(gRenderer);
        }

        SDL_Delay(1000 / (FPS * 2)); // The 2 is for doing the double of calculations
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
  // SDL_Surface *bgSurface = IMG_Load("assets/sprites/scenes/fondoOscuro.png");
  // SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(gRenderer, bgSurface);
  // SDL_Surface *menuBgSurface = IMG_Load("assets/sprites/scenes/breakoutFondo.png");
  // SDL_Texture *menuBgTexture = SDL_CreateTextureFromSurface(gRenderer, menuBgSurface);
  // SDL_Surface *menuTitleSurface = IMG_Load("assets/Images/title.png");
  // SDL_Surface *heartSurface = IMG_Load("assets/sprites/minecraft_heart.png");
  freeBricks(bricks, rows);
  SDL_FreeSurface(bgSurface);
  SDL_DestroyTexture(bgTexture);
  SDL_FreeSurface(menuBgSurface);
  SDL_DestroyTexture(menuBgTexture);
  SDL_FreeSurface(menuTitleSurface);
  SDL_DestroyTexture(menuTitleTexture);

  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();

  return 0;
}