bool renderHighscores (SDL_Renderer *renderer, TTF_Font *font) {
	bool success = true;
	unsigned short n;
	Highscore *hs = getHighscores(&n);
	SDL_Rect rect = {20, 20, 600, 440};
	SDL_Color color;
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 10);
  SDL_RenderFillRect(renderer, &rect);
	char text[100];
	strcpy(text, "High Scores");
	rect.x = 200;
	rect.y = 50;
	rect.w = 22 * strlen(text);
	rect.h = 50;
	color.r = 200;
	color.g = color.b = 0;
	if (renderText(text, rect, renderer, font, color))
		success = false;
	rect.y += 100;
	color.r = color.g = color.b = 235;
	rect.x = 30;
	rect.h = 27;
	for (unsigned short i=0;i<5;++i) {
		if (i < n)
			sprintf(text, "%2d. %20s %9d", i+1, (hs+i)->name, (hs+i)->val);
		else
			sprintf(text, "%2d.                       ", i+1);
		rect.w = strlen(text) * 13;
		renderText(text, rect, renderer, font, color);
		rect.y += 50;
	}
	return success;
}