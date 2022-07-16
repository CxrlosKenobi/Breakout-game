//* Paddle *//

bool InitPaddle(
  Paddle *paddle,
  SDL_Renderer **gRenderer,
  SDL_Surface **surface,
  SDL_Texture **texture
) {

  paddle -> sprite_path = "assets/dude.png";
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

  paddle -> rect.w /= 3; // Downscaling
  paddle -> rect.h /= 3; // Downscaling

  return true;
}
