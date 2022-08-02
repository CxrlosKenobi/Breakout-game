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
#include "modules/Paddle/controller.h"
#include "modules/CollidingBall/CollidingBall.h"
#include "modules/Bricks/Bricks.h"
#include "utils/utils.h"
#include "views/menu.h"
#include "views/credits.h"
#include "utils/highscores.h"
#include "views/win.h"
//
#define FPS (50)
#define SPEED (600)

const unsigned short WINDOW_WIDTH = 640;
const unsigned short WINDOW_HEIGHT = 480;

enum menu_option {menu, game, highscores, credits, quit, mute, win_view };

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

int main() {
  srand(time(NULL));
  bool start = Initialize("Breakout!", WINDOW_WIDTH, WINDOW_HEIGHT, &gWindow, &gRenderer);
  if (!start) return 1;

  SDL_Surface *bgSurface = IMG_Load("assets/sprites/scenes/fondoOscuro.png");
  SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(gRenderer, bgSurface);
  SDL_Surface *menuBgSurface = IMG_Load("assets/sprites/scenes/breakoutFondo.png");
  SDL_Texture *menuBgTexture = SDL_CreateTextureFromSurface(gRenderer, menuBgSurface);

  SDL_Surface *creditsSurface = IMG_Load("assets/sprites/scenes/fondoMenosOscuro.png");
  SDL_Texture *creditsTexture = SDL_CreateTextureFromSurface(gRenderer, creditsSurface);

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
  minecraftFont = TTF_OpenFont("assets/fonts/DePixelHalbfett.ttf", 40);
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

  Brick **bricks;
  SDL_Surface *brickSurface[4];
  brickSurface[0] =  NULL;
  brickSurface[1] =  IMG_Load("assets/sprites/bricks/3.png");
  brickSurface[2] =  IMG_Load("assets/sprites/bricks/2.png");
  brickSurface[3] =  IMG_Load("assets/sprites/bricks/1.png");

  SDL_Texture *brickTextures[4];
  brickTextures[1] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[1]);
  brickTextures[2] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[2]);
  brickTextures[3] = SDL_CreateTextureFromSurface(gRenderer, brickSurface[3]);

  //Mixer setup
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

  // Paddle setup
  Paddle paddle;
  bool summon_paddle = initPaddle(
    &paddle,
    &gRenderer,
    &paddle.surface,
    &paddle.texture,
    WINDOW_WIDTH,
    WINDOW_HEIGHT
  );
  if (!summon_paddle) return 1;

  // Ball setup
  Ball *b = malloc(sizeof(Ball));
  unsigned short ballsAmount = 1;
  initBall(b, paddle);
  SDL_Surface *ballSurface = IMG_Load("assets/sprites/ball.png");
  SDL_Texture *ballTexture = SDL_CreateTextureFromSurface(gRenderer, ballSurface);

  int up = 0, down = 0, left = 0, right = 0;
  bool closeWindow = 0;
  bool pause = true;
  bool win = false;
  bool lose = true;
  unsigned frame = 0;
  Highscore possible_highscore;
  unsigned short view = menu;
  unsigned short hoveredOption = game;
  score.val = 0;

  SDL_Event gameEvent;
  char input[11];
  bool alert_input;
  SDL_Rect input_text_rect;
  input_text_rect.x = 140;
  input_text_rect.y = 220;
  // input_text_rect.w = 11 * 15;
  input_text_rect.h = 30;
  SDL_Color input_text_color;
  input_text_color.r = 240;
  input_text_color.g = 240;
  input_text_color.b = 240;
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
                // case SDL_SCANCODE_RETURN:
                //   if (hoveredOption==mute)
                //     Mix_VolumeMusic(0)==0 ? Mix_VolumeMusic(MIX_MAX_VOLUME/12) : Mix_VolumeMusic(0);
                //   else {
                //     view = hoveredOption;
                //     if (view = game) {
                //       centerPaddle(&paddle, WINDOW_WIDTH, WINDOW_HEIGHT);
                //       initBall(b, paddle);
                //       bricks = createRandomBrickMatrix(rows, cols, &bricks_amount);
                //       up = down = left = right = 0;
                //       pause = true;
                //       score.val = 0;
                //       win = false;
                //       lose = false;
                //       lives = 3;
                //       frame = true;
                //     }
                //   }
                  // break;
                }
              break;
            case SDL_KEYDOWN:
              switch (gameEvent.key.keysym.scancode) {
                case SDL_SCANCODE_RETURN:
                  if (hoveredOption == mute)
                    Mix_VolumeMusic(0)==0?Mix_VolumeMusic(MIX_MAX_VOLUME/12):Mix_VolumeMusic(0);
                  else {
                    view = hoveredOption;
                    centerPaddle(&paddle, WINDOW_WIDTH, WINDOW_HEIGHT);
                    initBall(b, paddle);
                    bricks = createRandomBrickMatrix(rows, cols, &bricks_amount);
                    up = down = left = right = 0;
                    pause = true;
                    score.val = 0;
                    win = false;
                    lose = false;
                    lives = 3;
                    frame = 1;
                    // THIS IS JUST FOR TESTING PLEASE DELETE OR COMMENT BEFORE ANY COMMIT, DON'T BE STUPID YOU FCKIN IDIOT
                    // if (view == game) {
                    //   view = win_view;
                    //   score.val = 255;
                    //   *input = '\0';
                    //   SDL_StartTextInput();
                  }
                  break;
                  }
                }
              break;
        }
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, menuBgTexture, NULL, NULL);
        SDL_RenderCopy(gRenderer, menuTitleTexture, NULL, &menuTitleRect);
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
                case SDL_SCANCODE_Q:
                  view = menu;
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
          movePaddle(&paddle, up, down, left, right, WINDOW_WIDTH, WINDOW_HEIGHT, SPEED);

          // Update Balls state and calculate collisions
          for (unsigned short i = 0; i < ballsAmount; ++i) {
            if (manageWallCollision(b+i, &view, WINDOW_WIDTH, WINDOW_HEIGHT,sounds.bounce)) {
              centerPaddle(&paddle, WINDOW_WIDTH, WINDOW_HEIGHT);
              initBall(b, paddle);
              up = down = left = right = 0;
              pause = true;
              frame = false;
              lives--;
              Mix_PlayChannel(-1, brickSound, 0);
            }

            if(managePaddleCollision(b+i, paddle))
              Mix_PlayChannel(-1, bounce, 0);
            if (manageBricksCollision(bricks, b+i, WINDOW_WIDTH, 2*WINDOW_HEIGHT/5, rows, cols, 0, 0, &score.val, &bricks_amount)) {
              Mix_PlayChannel(-1, brickSound, 0);
            }
            (b+i)->pos.x += ((b+i)->vel.x) / 2;
            (b+i)->pos.y += ((b+i)->vel.y) / 2;
          }
          if (!bricks_amount) {
            win = true;
          }
          if (!bricks_amount)
            win = true;

          if (!lives) {
            lose = true;
          }
          if (win) {
            view = win_view;
            *input = '\0';
            alert_input = false;
            SDL_StartTextInput();
          } else if (lose) {
            view = menu;
          }

        }
        frame++;

        if (frame%2) {
          SDL_RenderClear(gRenderer);
          SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);
          SDL_RenderCopy(gRenderer, paddle.texture, NULL, &paddle.rect);
          renderBricks(bricks, gRenderer, brickTextures, WINDOW_WIDTH, 2*WINDOW_HEIGHT/5, rows, cols, 0, 0);
          renderBall(*b, gRenderer, ballTexture);
          sprintf(score.string, "Score: %4d", score.val);
          renderText(score.string, score.rect, gRenderer, minecraftFont, score.color); // render Score
          renderLives(gRenderer, lives, heartTexture);
          if (pause && ((frame%180) < 90)) {
            SDL_Rect rect = {230, 240, 11 * 17, 30};
            SDL_Color color = {0, 0, 0};
            renderText("Press SPACE", rect, gRenderer, minecraftFont, color);
          }
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
            case SDL_KEYDOWN:
              switch (gameEvent.key.keysym.scancode) {
              case SDL_SCANCODE_Q:
                view = menu;
                break;
              }
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
        SDL_RenderCopy(gRenderer, creditsTexture, NULL, NULL);
        renderCredits(gRenderer, minecraftFont);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000 / FPS);
        break;
      case quit:
        closeWindow = true;
        break;
      case win_view:
        possible_highscore.val = score.val;
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
          if (ev.type == SDL_QUIT) {
            closeWindow = true;
          } else if (ev.type == SDL_TEXTINPUT || ev.type == SDL_KEYDOWN) {
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && strlen(input) > 0)
              input[strlen(input)-1] = '\0';
            else if (ev.type == SDL_TEXTINPUT && strlen(input) < 11)
              strcat(input, ev.text.text);
            else if (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_RETURN) {
              if (validInput(input)) {
                strcpy(possible_highscore.name, input);
                SDL_StopTextInput();
                if (managePossibleNewHighscore(possible_highscore))
                  printf("That's a new Highscore :D\n");
                else 
                  printf("That's not a new Highscore\n");
                view = menu;
              } else {
                alert_input = true;
              }
            }
          }
        }
        SDL_RenderClear(gRenderer);
        input_text_rect.w = strlen(input) * 15;
        SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);
        renderAskingInformation(gRenderer, alert_input, minecraftFont);
        if (strlen(input) > 0) {
          renderText(input, input_text_rect, gRenderer, minecraftFont, input_text_color);
        }
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000 / FPS);
    }
  }
  freeBricks(bricks, rows);
  SDL_FreeSurface(bgSurface);
  SDL_DestroyTexture(bgTexture);
  SDL_FreeSurface(menuBgSurface);
  SDL_DestroyTexture(menuBgTexture);
  SDL_FreeSurface(menuTitleSurface);
  SDL_DestroyTexture(menuTitleTexture);
  SDL_FreeSurface(heartSurface);
  SDL_DestroyTexture(heartTexture);
  SDL_FreeSurface(creditsSurface);
  SDL_DestroyTexture(creditsTexture);

  //Free mixer
  Mix_FreeMusic(music);
  Mix_FreeChunk(bounce);
  Mix_FreeChunk(selectionSound);
  Mix_FreeChunk(brickSound);


  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();

  return 0;
}