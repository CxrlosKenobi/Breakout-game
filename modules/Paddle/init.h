//* Paddle *//
bool initPaddle(
  Paddle *paddle,
  SDL_Renderer **gRenderer,
  SDL_Surface **surface,
  SDL_Texture **texture,
  int WINDOW_WIDTH,
  int WINDOW_HEIGHT
) {

  paddle -> sprite_path = "assets/sprites/paddle.png";
  *surface = IMG_Load(paddle -> sprite_path);
	if (!*surface) {
    printf("Error[-1]: %s\n", IMG_GetError());
    return false;
  }

  *texture = SDL_CreateTextureFromSurface(*gRenderer, *surface);
  SDL_FreeSurface(*surface);
  if (!*texture) {
    printf("Error: %s\n", SDL_GetError());
    return false;
  }

  paddle -> surface = *surface;
  paddle -> texture = *texture;

  SDL_QueryTexture(
    paddle -> texture,
    NULL, NULL, // format, access
    &paddle -> rect.w,
    &paddle -> rect.h
  );


  // Place the paddle in the center of the screen
  paddle -> xPos = (WINDOW_WIDTH / 2) - (paddle -> rect.w / 2);
  paddle -> yPos = (WINDOW_HEIGHT / 2) - (paddle -> rect.h / 2);
  paddle -> speed.x = 0;

  return true;
}
