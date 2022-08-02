#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void renderMenu (unsigned short option, SDL_Renderer* renderer, TTF_Font* font) {
	SDL_Color color = {0, 0, 0};
	unsigned short letter_width = 20;
	SDL_Rect rect;
	rect.y = 270;
	rect.w = 5 * letter_width;
	rect.x = (640 - rect.w) / 2;
	rect.h = 33;
	if (option == 1)
		color.r = color.g = color.b = 100;
	renderText("START", rect, renderer, font, color);

 letter_width = 15;
	rect.y += 50;
	rect.w = 11 * letter_width;
	rect.x = (640 - rect.w) / 2;
	rect.h = 30;
	color.r = color.g = color.b = 0;
	if (option == 2)
		color.r = color.g = color.b = 100;
	renderText("HIGH SCORES", rect, renderer, font, color);

	rect.y += 50;
	rect.w = 7 * letter_width;
	rect.x = (640 - rect.w) / 2;
	color.r = color.g = color.b = 0;
	if (option == 3)
		color.r = color.g = color.b = 100;
	renderText("CREDITS", rect, renderer, font, color);

	letter_width = 13;
	rect.h = 26;
	rect.y += 50;
	rect.w = 4 * letter_width;
	rect.x = (640 - rect.w) / 2;
	color.r = color.g = color.b = 0;
	if (option == 4)
		color.r = color.g = color.b = 100;
	renderText("QUIT", rect, renderer, font, color);

	letter_width = 15;
	;
	rect.y += 0;
	rect.w = 4 * letter_width;
	rect.x = (640 - rect.w)-30 ;
	color.r = color.g = color.b = 0;
	if (option == 5)
		color.r = color.g = color.b = 100;
	renderText("MUTE", rect, renderer, font, color);
}