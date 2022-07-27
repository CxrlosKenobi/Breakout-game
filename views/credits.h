#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void renderCredits (SDL_Renderer *renderer, TTF_Font *font) {
	SDL_Color color;
	SDL_Rect rect;
	color.r = color.g = color.b = 0;
	color.a = 255;
	const unsigned short letter_width = 15;
	rect.x = 50;
	rect.y = 50;
	char* text = "Developed by";
	rect.h = 60;
	rect.w = letter_width * strlen(text) * 2;
	renderText(text, rect, renderer, font, color);
	
	// rect.x = 
	// SDL_RenderCopy(renderer, texture, NULL, NULL);
}