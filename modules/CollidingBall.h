typedef struct {
	int x;
	int y;
	int special;
} Ball;

void moveBall (Ball *b, int x, int y) {
	b->x += x;
	b->y += y;
}
