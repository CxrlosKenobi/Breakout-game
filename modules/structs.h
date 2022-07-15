#include <SDL2/SDL.h>

typedef struct {
	float x;
	float y;
} Velocity;

typedef struct {
	unsigned short health;
	short power;
} Brick;

typedef struct {
	SDL_Rect rect;
	Velocity vel;
	unsigned short radius;
} Ball;

typedef struct {
	SDL_Rect rect;
	Velocity vel;
	short power;
} Base;
