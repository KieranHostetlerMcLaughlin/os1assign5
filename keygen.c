#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	srand(time(NULL));
	char numArgsErr[] = "ERROR: Invalid number of arguments.\n";
	char evalToZero[] = "ERROR: atoi evaluates to 0.\n";
	if (argc != 2) {
		write(2, numArgsErr, strlen(numArgsErr));
		exit(1);
	}
	int amtChars = atoi(argv[1]);
	if (amtChars == 0) {
		write(2, evalToZero, strlen(evalToZero));
		exit(1);
	}
	char key[amtChars];
	int i = 0;
	for (i; i < amtChars; i++) {
		int randInt = rand() % 27;
		if (randInt == 26) {
			key[i] = ' ';
		}
		else {
			key[i] = 'A' + randInt;
		}
	}
	write(1, key, amtChars);
	write(1, "\n", 1);
}
