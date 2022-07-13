typedef struct {
	int x;
	int y;
	int vel;
} Base;

void moveBase (Base* b, int x, int y) {
	b->x += x;
	b->y += y;
}
