#include <stdlib.h>
#include <stdbool.h>

void manageWallCollision (Ball *b, bool *gameOver, unsigned short gameWidth, unsigned short gameHeight) {
  int limInfx = b->rect.x - b->radius;
  int limSupx = b->rect.x + b->radius;
  int limInfy = b->rect.y - b->radius;
  int limSupy = b->rect.y + b->radius;
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
		b->rect.x += b->vel.x;
		b->rect.y += b->vel.y;
	}
}

void initBall (Ball *b) {
	const int maxv = 2;
	const int minv = -2;
  const unsigned radius = 25;
	b->rect.w = radius * 2;
	b->rect.h = radius * 2;
	b->rect.x = 210 - (b->rect.w)/2;
	b->rect.y = 400 - (b->rect.h)/2;
	b->vel.x = rand()%(maxv-minv) + minv;
  printf("vel.x is : %d\n", b->vel.x);
	b->vel.y = ((maxv-minv)*(maxv-minv)) - (b->vel.x)*(b->vel.x);
	b->vel.y = sqrt(b->vel.y);
	b->vel.y *= -1;
  b->radius = radius;
}


void renderBall (Ball b, SDL_Renderer *renderer, SDL_Texture *texture) {
  SDL_RenderCopy(renderer, texture, NULL, &b.rect);
}
