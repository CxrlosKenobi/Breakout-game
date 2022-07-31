#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef const unsigned short cus;

// Returns true in lost
bool manageWallCollision (Ball *b, unsigned short *view, unsigned short gameWidth, unsigned short gameHeight) {
  int limInfx = b->pos.x - b->radius;
  int limSupx = b->pos.x + b->radius;
  int limInfy = b->pos.y - b->radius;
  int limSupy = b->pos.y + b->radius;
  // if (limInfx <= 0 || gameWidth <= limSupx)
  //   b->vel.x *= -1;
  if (limInfx <= 0)
    (b->vel.x) = fabs(b->vel.x);
  else if (gameWidth <= limSupx)
    (b->vel.x) = fabs(b->vel.x) * -1;
  if (limInfy <= 0)
    b->vel.y = fabs(b->vel.y);
  if (limSupy >= gameHeight) // loses a life
    return true;
  return false;
}

bool manageInferiorBrickCollision (Brick **bricks, const Ball b, const unsigned distance, const unsigned n, bool x_axis) {
  cus margin = 7;
  short limInf;
  short limSup;
  short half;
  unsigned short x;
  unsigned short y;
  const int crit = distance - margin;
  if (x_axis) {
    limInf = limSup = half = b.pos.x;
    y = b.pos.y;
  } else {
    limInf = limSup = half = b.pos.y;
    x = b.pos.x;
  }

  if (limSup < crit) return false;

  limInf -= b.radius;
  limSup += b.radius;
  limInf %= distance;
  half %= distance;
  limSup %= distance;
  if (!limInf) limInf += distance;
  if (!half) half += distance;
  if (!limSup) limSup += distance;
  return false;

  // if ((crit < limInf || crit < half || crit < limSup) && )
  // if (crit < limInf && )

  // if (!limInf || )
}

bool manageBricksCollision (Brick **bricks, Ball *b, cus WINDOW_WIDTH, cus WINDOW_HEIGHT, cus rows, cus cols, cus MARGINX, cus  MARGINY, unsigned *score, unsigned *bricks_amount) {
  cus margin = 7;
  const short limInfx = b->pos.x - (b->radius) - MARGINX;
  const short limSupx = b->pos.x + (b->radius) - MARGINX;
  const short limInfy = b->pos.y - (b->radius) - MARGINY;
  const short limSupy = b->pos.y + (b->radius) - MARGINY;
  bool collided = false;
  unsigned short x, y;
  if (limInfy < WINDOW_HEIGHT - 2*MARGINY && limInfx < WINDOW_WIDTH - 2*MARGINX) {
    cus brick_width = (WINDOW_WIDTH - 2*MARGINX)/cols;
    cus brick_height = (WINDOW_HEIGHT - 2*MARGINY)/rows;
    if (b->vel.x < 0) { // choca por derecha
      // if ((brick_width-margin <= limInfx%brick_width && limInfx%brick_width <= brick_width) || ((!limInfx%brick_width) && limInfx > 0)) {
      if ((brick_width-limInfx%brick_width <= margin) || (!(limInfx%brick_width) && limInfx > 0)) {
        x = limInfx/brick_width;
        if (!(limInfx%brick_width))
          x--;
        if (0 <= x && x < cols) {
          y = limInfy/brick_height;
          if (0 <= y && y < rows) {
            if (bricks[y][x].health) {
              (b->vel.x) = fabs(b->vel.x);
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
              b->vel.x = fabs(b->vel.x);
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
    else if (b->vel.x > 0) { // choca por izquierda
      if ((0 <= limSupx && limSupx%brick_width <= margin && limSupx < WINDOW_WIDTH-MARGINX) || (!limSupx%brick_width && limSupx < WINDOW_WIDTH-MARGINX)) {
        x = limSupx/brick_width;
        if (0 <= x && x < cols) {
          y = limInfy/brick_height;
          if (0 <= y && y < rows) {
            if (bricks[y][x].health) {
              collided = true;
              b->vel.x = fabs(b->vel.x) * -1;
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
              (b->vel.x) = fabs(b->vel.x) * -1;
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
    if (b->vel.y < 0) { // choca por abajo
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
              b->vel.y = fabs(b->vel.y);
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
              b->vel.y = fabs(b->vel.y);
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
    else if (b->vel.y > 0) { // choca por arriba
      if ((0 <= limSupy && limSupy%brick_height <= margin && limSupy < WINDOW_HEIGHT-2*MARGINY) || (!limSupy%brick_height && limSupy < WINDOW_HEIGHT-2*MARGINY)) {
        // printf("choca por arriba\n");
        y = limSupy/brick_height;
        if (0 <= y && y < rows) {
					x = limInfx/brick_width;
          if (0 <= x && x < cols) {
            if (bricks[y][x].health) {
              b->vel.y = fabs(b->vel.y) * -1;
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
              b->vel.y = fabs(b->vel.y) * -1;
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

void managePaddleCollision (Ball *b, Paddle p) {
  const unsigned short marginx = 6;
  const unsigned short marginy = 8;
  if ((p.rect.y-p.rect.h <= b->pos.y - marginy && b->pos.y <= p.rect.y) && (p.rect.x-marginx <= b->pos.x && b->pos.x <= p.rect.x+p.rect.w+marginx))
    b->vel.y = fabs(b->vel.y) * -1;
}

void initBall (Ball *b, cus gameWidth, cus gameHeight) {
  const short int vel = 7;
  short int maxv = 4;
  short int minv = -1 * maxv;
  b->radius = 6;
  b->pos.x = gameWidth / 2;
  b->pos.y = 4 * gameHeight / 5;
  (b->vel.x) = rand()%(maxv-minv);
  // printf("velocity of ball initialized in %lf\n", (b->vel.x));
  b->vel.x += minv;
  if (!(b->vel.x))
    (b->vel.x)--;
  b->vel.y = (vel*vel) - (b->vel.x)*(b->vel.x);
  b->vel.y = sqrt(b->vel.y);
  // b->vel.y = round(b->vel.y);
  b->vel.y *= -1;
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
