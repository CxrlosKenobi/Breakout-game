#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef const unsigned short cus;

void manageWallCollision (Ball *b, bool *gameOver, unsigned short gameWidth, unsigned short gameHeight) {
  int limInfx = b->pos.x - b->radius;
  int limSupx = b->pos.x + b->radius;
  int limInfy = b->pos.y - b->radius;
  int limSupy = b->pos.y + b->radius;
  if (limInfx <= 0 || gameWidth <= limSupx)
    b->vel.x *= -1;
  if (limInfy <= 0)
    b->vel.y *= -1;
  if (limSupy >= gameHeight)
    *gameOver = true;
}

void manageBricksCollision (Brick **bricks, Ball *b, cus WINDOW_WIDTH, cus WINDOW_HEIGHT, cus rows, cus cols, cus MARGINX, cus  MARGINY) {
  cus margin = 7;
  const short limInfx = b->pos.x - (b->radius) - MARGINX;
  const short limSupx = b->pos.x + (b->radius) - MARGINX;
  const short limInfy = b->pos.y - (b->radius) - MARGINY;
  const short limSupy = b->pos.y + (b->radius) - MARGINY;
  bool collided = false;
  if (limInfy < WINDOW_HEIGHT - 2*MARGINY && limInfx < WINDOW_WIDTH - 2*MARGINX) {
    cus brick_width = (WINDOW_WIDTH - 2*MARGINX)/cols;
    cus brick_height = (WINDOW_HEIGHT - 2*MARGINY)/rows;
    if (b->vel.x < 0) { // choca por derecha
      if ((brick_width-margin <= limInfx%brick_width && limInfx%brick_width <= brick_width) || ((!limInfx%brick_width) && limInfx > 0)) {
        if (bricks[limInfy/brick_height][limInfx/brick_width].health && limInfy/brick_height<rows && limInfx/brick_width<cols) {
          b->vel.x = abs(b->vel.x);
          collided = true;
          // printf("choque por derecha uno\n");
          bricks[limInfy/brick_height][limInfx/brick_width].health--;
        }
        else if (bricks[limSupy/brick_height][limInfx/brick_width].health && limSupy/brick_height<rows && limInfx/brick_width<cols) {
          // printf("choque por derecha dos\n");
          collided = true;
          b->vel.x = abs(b->vel.x);
          bricks[limSupy/brick_height][limInfx/brick_width].health--;
        }
      }
    }
    else if (b->vel.x > 0) { // choca por izquierda
      /* if ((!limSupx%brick_width && limSupx<WINDOW_WIDTH-MARGINX) || brick_width >= (limInfx%brick_width)+margin) { */
      if ((0 <= limSupx && limSupx%brick_width <= margin && limSupx < WINDOW_WIDTH-MARGINX) || (!limSupx%brick_width && limSupx < WINDOW_WIDTH-MARGINX)) {
        if (bricks[limInfy/brick_height][limSupx/brick_width].health && limInfy/brick_height<rows && limSupx/brick_width<cols) {
          // printf("choque por izquierda uno\n");
          collided = true;
          b->vel.x = abs(b->vel.x) * -1;
          bricks[limInfy/brick_height][limSupx/brick_width].health--;
          // printf("read element (%d,%d)\n\n", limInfy/brick_height, limSupx/brick_width);
        }
        else if (bricks[limSupy/brick_height][limSupx/brick_width].health && limSupy/brick_height<rows && limSupx/brick_width<cols) {
          // printf("choque por izquierda dos\n");
          // printf("read element (%d,%d)\n\n", limSupy/brick_height, limSupx/brick_width);
          collided = true;
          b->vel.x = abs(b->vel.x) * -1;
          bricks[limSupy/brick_height][limSupx/brick_width].health--;
        }
      }
    }
    if (b->vel.y < 0) { // choca por abajo
      if ((brick_height-margin <= limInfy%brick_height && limInfy%brick_height <= brick_height) || ((!limInfy%brick_height) && limInfy > 0)) {
        if (bricks[limInfy/brick_height][limInfx/brick_width].health && limInfy/brick_height<rows && limInfx%brick_width<cols) {
          // printf("choque por abajo\n");
          // printf("Chocaría con el bloque en la posicion (%d, %d)\n", limInfy/brick_height, limInfx/brick_width);
          b->vel.y = abs(b->vel.y);
          if (!collided)
            bricks[limInfy/brick_height][limInfx/brick_width].health--;
            collided = true;
        }
        else if (bricks[limInfy/brick_height][limSupx/brick_width].health && limInfy/brick_height<rows && limSupx/brick_width<cols) {
          // printf("o con el bloque en la posicion (%d, %d)\n", limInfy/brick_height, limSupx/brick_width);
          // printf("choque por abajo\n");
          b->vel.y = abs(b->vel.y);
          if (!collided)
            bricks[limInfy/brick_height][limSupx/brick_width].health--;
            collided = true;
        }
      }
    }
    else if (b->vel.y > 0) { // choca por arriba
      if ((0 <= limSupy && limSupy%brick_height <= margin && limSupy < WINDOW_HEIGHT-2*MARGINY) || (!limSupy%brick_height && limSupy < WINDOW_HEIGHT-2*MARGINY)) {
        // if (limSupy)
        if (bricks[limSupy/brick_height][limInfx/brick_width].health && limSupy/brick_height<rows && limInfx/brick_width<cols) {
          b->vel.y = abs(b->vel.y) * -1;
          if (!collided)
            bricks[limSupy/brick_height][limInfx/brick_width].health--;
        }
        else if (bricks[limSupy/brick_height][limSupx/brick_width].health && limSupy/brick_height<rows && limInfx/brick_width<cols) {
          b->vel.y = abs(b->vel.y) * -1;
          if (!collided)
            bricks[limSupy/brick_height][limSupx/brick_width].health--;
        }
      }
    }
  }
}

void managePaddleCollision (Ball *b, Paddle p) {
  const unsigned short marginx = 6;
  const unsigned short marginy = 8;
  if ((p.rect.y-p.rect.h <= b->pos.y - marginy && b->pos.y <= p.rect.y) && (p.rect.x-marginx <= b->pos.x && b->pos.x <= p.rect.x+p.rect.w+marginx))
    b->vel.y = abs(b->vel.y) * -1;
}

void updateBalls (Ball *b, unsigned short n, bool *gameOver, unsigned short gameWidth, unsigned short gameHeight, Paddle paddle) {
  for (unsigned short i=0;i<n;++i) {
    manageWallCollision(b+i, gameOver, gameWidth, gameHeight);
    managePaddleCollision(b+i, paddle);
    /* manageBricksCollision(b+i); */
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;
  }
}

void initBall (Ball *b, cus gameWidth, cus gameHeight) {
  const short int vel = 7;
  const short int maxv = 4;
  const short int minv = -1 * maxv;
  b->radius = 6;
  b->pos.x = gameWidth / 2;
  b->pos.y = 4 * gameHeight / 5;
  b->vel.x = rand()%(maxv-minv) + minv;
  if (!b->vel.x)
    b->vel.x--;
  b->vel.y = (vel*vel) - (b->vel.x)*(b->vel.x);
  b->vel.y = sqrt(b->vel.y);
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
