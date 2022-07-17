#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

void updateBalls (Ball *b, unsigned short n, bool *gameOver, unsigned short gameWidth, unsigned short gameHeight) {
	for (unsigned short i=0;i<n;++i) {
    manageWallCollision(b+i, gameOver, gameWidth, gameHeight);
    /* manageBaseCollision(b+i); */
    /* manageBricksCollision(b+i); */
		b->pos.x += b->vel.x;
		b->pos.y += b->vel.y;
	}
}

void initBall (Ball *b, unsigned short gameWidth, unsigned short gameHeight) {
  srand(time(0));
  const int vel = 7;
	const int maxv = 4;
	const int minv = -1 * maxv;
  b->radius = 6;
	b->pos.x = gameWidth / 2;
	b->pos.y = 4 * gameHeight / 5;
	b->vel.x = rand()%(maxv-minv) + minv;
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

void freeBalls (Ball *b, unsigned short n) {
  for (unsigned short i=0;i<n;++i) {
    free(b+i);
  }
}
