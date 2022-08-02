#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void renderCredits (SDL_Renderer *renderer, TTF_Font *font) {
	SDL_Color color;
	SDL_Rect rect;
	SDL_Rect seba;
	SDL_Rect alberto;
	SDL_Rect carlos;
	SDL_Rect menu;
	color.r = color.g = color.b = 0;
	color.a = 255;
	unsigned short letter_width = 15;
	char* text = "Developed by";
	rect.w = letter_width * strlen(text) * 2;
	rect.x = 60;
	rect.y = 50; //150
	rect.h = 50;
	renderText(text, rect, renderer, font, color);
	letter_width = 8;
	char* albertoText = "@AlbFR";
	alberto.h = 40;
	alberto.w = letter_width * strlen(albertoText) * 2;
	alberto.x = 540;
	alberto.y = 150; //150
	renderText(albertoText, alberto, renderer, font, color);
	char* carlosText = "@CxrlosKenobi";
	carlos.h = 40;
	carlos.w = letter_width * strlen(carlosText) * 2;
	carlos.x = 428;
	carlos.y = 200; //150
	renderText(carlosText, carlos, renderer, font, color);
	char* sebaText = "@SebaSinMas";
	seba.h = 40;
	seba.w = letter_width * strlen(sebaText) * 2;
	seba.x = 462;
	seba.y = 250; //150
	renderText(sebaText, seba, renderer, font, color);
	char* backMenu="Press Q to go back to menu";
	menu.h = 40;
	menu.w = letter_width * strlen(backMenu) * 2;
	menu.x = ((640 - menu.w) / 2)-44;
	menu.y = 400; //150
	renderText(backMenu, menu, renderer, font, color);



	// rect.x = 
	// SDL_RenderCopy(renderer, texture, NULL, NULL);
}