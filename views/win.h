#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

bool renderAskingInformation (SDL_Renderer *renderer, bool print_alert, TTF_Font *font) {
  bool success = true;
  SDL_Rect rect;
  SDL_Color color;
  // Lightblue backgorund rect
  rect.x = 80;
  rect.y = 140;
  rect.w = 440;
  rect.h = 200;
  SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
  SDL_RenderFillRect(renderer, &rect);

  // "Write your nickname" message
  char text[150];
  unsigned short letter_width = 19;
  strcpy(text, "Write your nickname:");
  color.a = 255;
  rect.x = 100;
  rect.y = 160;
  rect.w = strlen(text) * letter_width;
  rect.h = 40;
  color.r = color.g = color.b = 240;
  if (!renderText(text, rect, renderer, font, color))
    success = false;

  // Background rect of input text
	rect.x = 130;
  rect.y = 215;
  rect.w = 11 * 17;
  rect.h = 40;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &rect);

  if (print_alert) { // print alert, isn't obvious?
    letter_width = 8;
    color.g = color.b = 0; // makes red
    rect.x = 120;
    rect.y = 260;
    rect.h = 19;
    strcpy(text, "Your nick must not have spaces, tabs or endlines");
    rect.w = letter_width * strlen(text);
    renderText(text, rect, renderer, font, color);
    strcpy(text, "and must be between one and ten leters length");
    rect.w = letter_width * strlen(text);
    rect.y += 20;
    renderText(text, rect, renderer, font, color);
  }

  return success;
}