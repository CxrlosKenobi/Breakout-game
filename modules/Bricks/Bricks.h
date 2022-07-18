Brick** createRandomBrickMatrix (const unsigned short m, const unsigned short n) {
	Brick **bricks = malloc(m * sizeof(Brick*));
	for (unsigned short i=0;i<m;++i) {
		bricks[i] = malloc(n * sizeof(Brick));
    for (unsigned short j=0;j<n;++j) {
      if (rand()%3)
        bricks[i][j].health = 0;
      else
        bricks[i][j].health = rand() % 4;
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

void renderBricks (Brick **m, SDL_Renderer *renderer, SDL_Texture **texture, const unsigned short rows, const unsigned short cols, const unsigned short MATRIX_WIDTH, const unsigned short MATRIX_HEIGHT) {
  SDL_Rect *rect = malloc(sizeof(SDL_Rect));
  SDL_Texture *brickTexture;
  rect -> w = MATRIX_WIDTH / cols - 2;
  rect -> h = MATRIX_HEIGHT/ rows - 2;
  rect -> y = 0;
  for (unsigned short i=0;i<rows;++i) {
    rect -> x = 0;
    for (unsigned short j=0;j<cols;++j) {
      /* if (!m[i][j].health) */
      /*   continue; */
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
      rect -> x += rect -> w + 4;
    }
    rect -> y += rect -> h + 4;
  }
}
