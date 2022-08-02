//
void printBrickMatrix (Brick** m, unsigned short rows, unsigned short cols) {
  for (unsigned short i=0;i<rows;++i) {
    for (unsigned short j=0;j<rows;++j)
      printf("%d ", m[i][j].health);
    printf("\n");
  }
}

Brick** createRandomBrickMatrix (const unsigned short rows, const unsigned short cols, unsigned *bricks_amount) {
  *bricks_amount = 0;
	Brick **bricks = malloc(rows * sizeof(Brick*));
	for (unsigned short i = 0; i < rows; ++i) {
		bricks[i] = malloc(cols * sizeof(Brick));
    for (unsigned short j=0; j < cols; ++j) {
      if (rand() % 3)
        bricks[i][j].health = 0;
      else
        bricks[i][j].health = rand() % 4;
      if (bricks[i][j].health)
        (*bricks_amount)++;
      bricks[i][j].power = 0;
    }
	}
  return bricks;
}
void freeBricks (Brick **bricks, const unsigned short m) {
  for (unsigned short i=0;i<m;++i)
    free(bricks[i]);
  free(bricks);
}

void renderBricks (Brick **m, SDL_Renderer *renderer, SDL_Texture **texture, const unsigned short WINDOW_WIDTH, const unsigned short WINDOW_HEIGHT, const unsigned short rows, const unsigned short cols, const unsigned short MARGINX, const unsigned short MARGINY) {
  SDL_Rect *rect = malloc(sizeof(SDL_Rect));
  SDL_Texture *brickTexture;
  rect -> w = WINDOW_WIDTH - 2*MARGINX;
  rect -> w /= cols;

  rect -> h = WINDOW_HEIGHT - 2*MARGINY;
  rect -> h /= rows;
  rect -> y = MARGINY;
  for (unsigned short i=0;i<rows;++i) {
    rect -> x = MARGINX;
    for (unsigned short j=0;j<cols;++j) {
      switch (m[i][j].health) {
        case 0:
          brickTexture = NULL;
          break;
        case 1:
          brickTexture = texture[1];
          break;
        case 2:
          brickTexture = texture[2];
          break;
        case 3:
          brickTexture = texture[3];
          break;
        default:
          printf("Something strange has happened with bricks health\n");
          break;
      }
      SDL_RenderCopy(renderer, brickTexture, NULL, rect);
      rect -> x += rect->w;
    }
    rect -> y += rect->h;
  }
}
