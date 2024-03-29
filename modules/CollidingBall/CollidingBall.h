#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef const unsigned short cus; // CUS: const unsigned short

// Returns true in lost
bool manageWallCollision (Ball *b, unsigned short *view, unsigned short gameWidth, unsigned short gameHeight, Mix_Chunk *bounce) {
  int limInfx = b -> pos.x - b -> radius;
  int limSupx = b -> pos.x + b -> radius;
  int limInfy = b -> pos.y - b -> radius;
  int limSupy = b -> pos.y + b -> radius;
  // if (limInfx <= 0 || gameWidth <= limSupx)
  //   b -> vel.x *= -1;
  if (limInfx <= 0){
    Mix_PlayChannel(-1, bounce, 0);
    (b -> vel.x) = fabs(b -> vel.x);
  }
  else if (gameWidth <= limSupx){
    Mix_PlayChannel(-1, bounce, 0);
    (b -> vel.x) = fabs(b -> vel.x) * -1;
  }
  if (limInfy <= 0){
    Mix_PlayChannel(-1, bounce, 0);
    b -> vel.y = fabs(b -> vel.y);
  }
  if (limSupy >= gameHeight) // loses a life
    return true;
  return false;
}

bool manageBricksCollision (Brick **bricks, Ball *b, cus WINDOW_WIDTH, cus WINDOW_HEIGHT, cus rows, cus cols, cus MARGINX, cus  MARGINY, unsigned *score, unsigned *bricks_amount) {
  cus margin = 7;
  const short limInfx = b -> pos.x - (b -> radius) - MARGINX;
  const short limSupx = b -> pos.x + (b -> radius) - MARGINX;
  const short limInfy = b -> pos.y - (b -> radius) - MARGINY;
  const short limSupy = b -> pos.y + (b -> radius) - MARGINY;
  bool collided = false;
  unsigned short x, y;
  if (limInfy < WINDOW_HEIGHT - 2*MARGINY && limInfx < WINDOW_WIDTH - 2*MARGINX) {
    cus brick_width = (WINDOW_WIDTH - 2*MARGINX)/cols;
    cus brick_height = (WINDOW_HEIGHT - 2*MARGINY)/rows;
    if (b -> vel.x < 0) { // choca por derecha
      // if ((brick_width-margin <= limInfx%brick_width && limInfx%brick_width <= brick_width) || ((!limInfx%brick_width) && limInfx > 0)) {
      if ((brick_width-limInfx%brick_width <= margin) || (!(limInfx%brick_width) && limInfx > 0)) {
        x = limInfx/brick_width;
        if (!(limInfx%brick_width))
          x--;
        if (0 <= x && x < cols) {
          y = limInfy/brick_height;
          if (0 <= y && y < rows) {
            if (bricks[y][x].health) {
              (b -> vel.x) = fabs(b -> vel.x);
              collided = true;
              // printf("choque por derecha uno\n");
              bricks[y][x].health--;
              *score += 10;
              if (!bricks[y][x].health) {
                *score += 30;
                (*bricks_amount)--;
              }
              return true;
            }
          }
          y = limSupy/brick_height;
          if (0 <= y && y < rows && !collided) {
            if (bricks[y][x].health) {
              // printf("choque por derecha dos\n");
              collided = true;
              b -> vel.x = fabs(b -> vel.x);
              bricks[y][x].health--;
              (*score) += 10;
              if (!bricks[y][x].health) {
                (*score) += 30;
                (*bricks_amount)--;
              }
              return true;
            }
          }
        }
      }
    }
    else if (b -> vel.x > 0) { // choca por izquierda
      if ((0 <= limSupx && limSupx%brick_width <= margin && limSupx < WINDOW_WIDTH-MARGINX) || (!limSupx%brick_width && limSupx < WINDOW_WIDTH-MARGINX)) {
        x = limSupx/brick_width;
        if (0 <= x && x < cols) {
          y = limInfy/brick_height;
          if (0 <= y && y < rows) {
            if (bricks[y][x].health) {
              collided = true;
              b -> vel.x = fabs(b -> vel.x) * -1;
              bricks[y][x].health--;
              (*score) += 10;
              if (!bricks[y][x].health) {
                (*score) += 30;
                (*bricks_amount)--;
              }
              return true;
            }
          }
          y = limSupy/brick_height;
          if (0 <= y && y < rows) {
            if (bricks[y][x].health) {
              collided = true;
              (b -> vel.x) = fabs(b -> vel.x) * -1;
              bricks[y][x].health--;
              (*score) += 10;
              if (!bricks[y][x].health) {
                (*score) += 30;
                (*bricks_amount)--;
              }
              return true;
            }
          }
        }
      }
    }
    if (b -> vel.y < 0) { // choca por abajo
      if ((brick_height-limInfy%brick_height <= margin) || (!(limInfy%brick_height) && limInfy > 0)) {
        y = limInfy/brick_height;
        if (!(limInfy%brick_height))
          y--;
          // limInfy -= brick_height;
        if (0 <= y && y < rows) {
          x = limInfx/brick_width;
          if (0 <= x && x < cols) {
            if (bricks[y][x].health) {
              // printf("choque por abajo1\n");
              // printf("Chocaría con el bloque en la posicion (%d, %d)\n", limInfx/brick_width, limInfy/brick_height);
              b -> vel.y = fabs(b -> vel.y);
              if (!collided) {
                bricks[y][x].health--;
                collided = true;
                (*score) += 10;
                if (!bricks[y][x].health) {
                  (*score) += 30;
                  (*bricks_amount)--;
                }
                return true;
              }
            }
          }
          x = limSupx/brick_width;
          if (0 <= x && x < cols) {
            if (bricks[y][x].health) {
              // printf("choque por abajo2\n");
              // printf("Choque con el bloque en la posicion (%d, %d)\n", limSupx/brick_width, limInfy/brick_height);
              b -> vel.y = fabs(b -> vel.y);
              if (!collided) {
                bricks[y][x].health--;
                collided = true;
                (*score) += 10;
                if (!bricks[y][x].health) {
                  *score += 30;
                  (*bricks_amount)--;
                }
                return true;
              }
            }
          }
        }
      }
    }
    else if (b -> vel.y > 0) { // choca por arriba
      if ((0 <= limSupy && limSupy%brick_height <= margin && limSupy < WINDOW_HEIGHT-2*MARGINY) || (!limSupy%brick_height && limSupy < WINDOW_HEIGHT-2*MARGINY)) {
        // printf("choca por arriba\n");
        y = limSupy/brick_height;
        if (0 <= y && y < rows) {
					x = limInfx/brick_width;
          if (0 <= x && x < cols) {
            if (bricks[y][x].health) {
              b -> vel.y = fabs(b -> vel.y) * -1;
              if (!collided) {
                bricks[y][x].health--;
                collided = true;
                *score += 10;
                if (!bricks[y][x].health) {
                  *score += 30;
                  (*bricks_amount)--;
                }
                return true;
              }
            }
          }
          x = limSupx/brick_width;
          if (0 <= x && x < cols) {
            if (bricks[y][x].health) {
              b -> vel.y = fabs(b -> vel.y) * -1;
              if (!collided) {
                bricks[y][x].health--;
                collided = true;
                *score += 10;
                if (!bricks[y][x].health) {
                  (*score) += 30;
                  (*bricks_amount)--;
                }
                return true;
              }
            }
          }
        }
      }
    }
  }
  return false;
}

bool managePaddleCollision (Ball *ball, Paddle pad) {
  const unsigned short marginx = 7;
  const unsigned short marginy = 9;

  // When the ball hits anywhere in the first third of the left side of the upper paddle
  if (
    ((pad.rect.y - pad.rect.h)  <=  (ball -> pos.y - marginy)  &&  ball -> pos.y <= pad.rect.y)  &&
    (pad.rect.x - marginx  <=  ball -> pos.x  &&  ball -> pos.x  <=  pad.rect.x + pad.rect.w/3 + marginx)
  ) {
    ball -> vel.y = fabs(ball -> vel.y) * -1;
    ball -> vel.x = fabs(ball -> vel.x) * -1;
    return true;
  }

  // When the ball hits anywhere in the second third of the left side of the upper paddle
  else if (
    ((pad.rect.y - pad.rect.h)  <=  (ball -> pos.y - marginy)  &&  ball -> pos.y <= pad.rect.y)  &&
    (pad.rect.x + pad.rect.w/3 - marginx  <=  ball -> pos.x  &&  ball -> pos.x  <=  pad.rect.x + pad.rect.w*2/3 + marginx)
  ) {
    ball -> vel.y = fabs(ball -> vel.y) * -1;
    return true;
  }
  
  // When the ball hits anywhere in the third third of the left side of the upper paddle
  else if (
    ((pad.rect.y - pad.rect.h)  <=  (ball -> pos.y - marginy)  &&  ball -> pos.y <= pad.rect.y)  &&
    (pad.rect.x + pad.rect.w*2/3 - marginx  <=  ball -> pos.x  &&  ball -> pos.x  <=  pad.rect.x + pad.rect.w + marginx)
  ) {
    // Mutate the ball to orientate it to the right
    ball -> vel.y = fabs(ball -> vel.y) * -1;
    ball -> vel.x = fabs(ball -> vel.x);
    return true;
  } else return false;
}

void initBall (Ball *b, Paddle pad) {
  const short int vel = 7;
  short int maxv = 4;
  short int minv = -1 * maxv;
  b -> radius = 6;

  b -> pos.x = pad.rect.x + pad.rect.w/2;
  b -> pos.y = pad.rect.y - b -> radius;

  b -> vel.x = rand()  %(maxv - minv);
  b -> vel.x += minv;
  if (!(b -> vel.x))
    (b -> vel.x)--;
  b -> vel.y = (vel * vel) - (b -> vel.x) * (b -> vel.x);
  b -> vel.y = sqrt(b -> vel.y);
  b -> vel.y *= -1;
}

void renderBall (Ball b, SDL_Renderer *renderer, SDL_Texture *texture) {
  SDL_Rect *rect = malloc(sizeof(SDL_Rect));
  rect->w = b.radius * 2;
  rect->h = b.radius * 2;
  rect->x = b.pos.x - b.radius;
  rect->y = b.pos.y - b.radius;
  SDL_RenderCopy(renderer, texture, NULL, rect);
  free(rect);
}

void renderBallSquare (Ball b, SDL_Renderer *renderer) {
  SDL_Rect *rect = malloc(sizeof(SDL_Rect));
  rect->w = b.radius * 2;
  rect->h = b.radius * 2;
  rect->x = b.pos.x - b.radius;
  rect->y = b.pos.y - b.radius;
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, rect);
  free(rect);
}

void freeBalls (Ball *b, cus n) {
  for (unsigned short i=0;i<n;++i) {
    free(b+i);
  }
}
