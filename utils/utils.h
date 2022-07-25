#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


bool renderText (char* text, int x, int y, SDL_Renderer* renderer, TTF_Font* font) {
	SDL_Color White = {255, 255, 255};
	SDL_Surface* surfaceMessage = NULL;
	surfaceMessage = TTF_RenderText_Solid(font, text, White);
	// if (surfaceMessage == NULL)
	// 	printf("An error has occured during the creation of surface message\n");
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect msgRect;
	msgRect.x = x;
	msgRect.y = y;
	msgRect.w = 300;
	msgRect.h = 200;
	SDL_RenderCopy(renderer, Message, NULL, &msgRect);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

bool LoadSurfaceTextureIMG (
	const char *filePath,
	SDL_Surface **objSurface, 
	SDL_Texture **objTexture,
	SDL_Renderer **gRenderer
	) {

	*objSurface = IMG_Load(filePath);
	if (!*objSurface) {
		printf("Error[-1]: %s\n", IMG_GetError());
		return false;
	}

	*objTexture = SDL_CreateTextureFromSurface(*gRenderer, *objSurface);
	if (!*objTexture) {
		printf("Error[-1]: %s\n", SDL_GetError());
		return false;
	}
	return true;
}



void destroyAll(SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *surface) {
	if (window != NULL)
		SDL_DestroyWindow(window);
	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if (surface != NULL)
		SDL_FreeSurface(surface);

	SDL_Quit();
};
