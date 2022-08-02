#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
	char name[11];
	unsigned val;
} Highscore;

Highscore* getHighscores (unsigned short *amount, Highscore candidate) {
	FILE *file = NULL;
	file = fopen("utils/highscores.txt", "r");
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
		// printf("%s: %u\n", name, score);
		strcpy((arr+i)->name, name);
		(arr+i)->val = score;
	}
	fclose(file);
	return arr;
}

void killHighscores (Highscore *hs) {
	free(hs);
}

char* getHighscoreFile () {
	FILE *file = NULL;
	file = fopen("utils/highscores.txt", "r");
	if (file == NULL)
		return NULL;
	char *string_file = malloc(sizeof(char) * 5 * 101); // 5 lines of 100 chars
	*string_file = '\0';
	char *line = malloc(sizeof(char) * 101);
	fscanf(file, "%[^\n] ", line);
	do {
		strcat(line, "\n");
		strcat(string_file, line);
	} while (fscanf(file, "%[^\n] ", line) != EOF);
	free(line);
	fclose(file);
	return string_file;
}

char unsignedToChar (unsigned n) {
	switch (n) {
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
	}
}

bool writeNewHighscore (char* string_file, unsigned short pos, Highscore hs, bool delete_last, unsigned short amount) {
	FILE *file = NULL;
	file = fopen("utils/highscores.txt", "w");
	unsigned short line = 0;
	unsigned short i = 0;
	// if (!delete_last)
	// 	(*string_file)++;
	if (!delete_last) {
		unsigned short max;
		max = *string_file - '0';
		max++;
		*string_file = unsignedToChar(max);
	}
	while (i < 300) {
		while (string_file[i] != '\n') {
			fprintf(file, "%c", string_file[i]);
			i++;
		}
		fprintf(file, "\n");
		i++;
		line++;
		if (line == pos+1) {
			fprintf(file, "%s %u\n", hs.name, hs.val);
			line++;
			break;
		}
	}
	while (line != amount+2-delete_last) {
		while (string_file[i] != '\n') {
			fprintf(file, "%c", string_file[i]);
			i++;
		}
		fprintf(file, "\n");
		i++;
		line++;
	}
	fclose(file);
	free(string_file);
}

// candidate.name MUST NOT contain any space or endline, etc
bool managePossibleNewHighscore (Highscore candidate) {
	const unsigned short maxHighScores = 3;
	unsigned short amount;
	Highscore *hs = getHighscores(&amount, candidate);

	if (hs == NULL)
		return false;

	for (unsigned short i=0;i<amount;++i) {
		if (candidate.val > (hs+i)->val) {
			getHighscoreFile();
			writeNewHighscore(getHighscoreFile(), i, candidate, (maxHighScores == amount), amount);
			killHighscores(hs);
			return true;
		}
	}
	if (amount < maxHighScores) {
		writeNewHighscore(getHighscoreFile(), amount, candidate, false, amount);
		return true;
	}

	printf("That's not a highscore\n");
	killHighscores(hs);
  return false;
}