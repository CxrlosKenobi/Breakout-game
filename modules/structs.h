#include <SDL2/SDL_mixer.h>
// Global
typedef struct {
	float x;
	float y;
} Position;

typedef struct {
	float x;
	float y;
} Speed;

// Base Paddle
typedef struct {
	// SDL stuff
	char *sprite_path;
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Rect rect;

	// Physics
	float xPos;
	float yPos;
	Speed speed;
	short power;
} Paddle; // Manage from dynamic memory for better performance

// Background
typedef struct {
	SDL_Rect rect;
	SDL_Surface *surface;
	SDL_Texture *texture;
} Scene;


// Bricks
typedef struct {
	unsigned short health;
	short power;
} Brick;

typedef struct {
	Position pos;
	Speed vel;
	unsigned short radius;
} Ball;

typedef struct {
	Mix_Chunk *bounce;
} Sound;
