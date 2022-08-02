#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
	char name[30];
	unsigned val;
} Highscore;

Highscore* getHighscores (unsigned short *amount) {
	FILE *file = NULL;
	file = fopen("highscores.txt", "r");
	if (file == NULL) {
		printf("You should see if highscores.txt exists :|\n");
		return NULL;
	}
	char name[50];
	unsigned score;
	fscanf(file, "%[^\n]", name);
	*amount = strtoul(name, NULL, 10);
	Highscore *arr = malloc(sizeof(Highscore) * *amount);
	for (unsigned short i=0;i<*amount;++i) {
		fscanf(file, "%s %u ", name, &score);
		printf("%s: %u\n", name, score);
		strcpy((arr+i)->name, name);
		(arr+i)->val = score;
	}
	fclose(file);
	return arr;
}

void killHighscores (Highscore *hs) {
	free(hs);
}

unsigned short managePossibleNewHighscore (unsigned candidateScore) {
	const unsigned short maxHighScores = 5;
	unsigned short amount;
	Highscore *hs = getHighscores(&amount);

	if (hs == NULL)
		return false;

	printf("amount of highscores: %u\n", amount);

	if (amount < maxHighScores) {
		printf("That's a highscore :D\n");
		return true;
	}

	unsigned score;
	for (unsigned short i=0;i<amount;++i) {
		if (candidateScore > (hs+i)->val) {
			printf("That's a highscore :D\n");
			killHighscores(hs);
			return i;
		}
	}
	if (amount < maxHighScores)
		return amount+1;

	printf("That's not a highscore\n");
	killHighscores(hs);
  return 0;
}

int main () {
	managePossibleNewHighscore(500);
  return 0;
}