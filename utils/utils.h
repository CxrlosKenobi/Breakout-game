#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


bool renderText (char* text, SDL_Rect msgRect, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {
	bool val = false;
	SDL_Surface* surfaceMessage = NULL;
	surfaceMessage = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	if (!SDL_RenderCopy(renderer, Message, NULL, &msgRect))
		val = true;
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
	return val;
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
