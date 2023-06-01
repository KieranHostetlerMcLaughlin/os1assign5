#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	srand(time(NULL)); //initalize randomness
	char numArgsErr[] = "ERROR: Invalid number of arguments.\n";
	char evalToZero[] = "ERROR: atoi evaluates to 0.\n";
	if (argc != 2) {
		write(2, numArgsErr, strlen(numArgsErr));
		exit(1);
	}
	int amtChars = atoi(argv[1]);
	if (amtChars == 0) {
		//yeah I could probably put this in a function
		//I didn't though
		write(2, evalToZero, strlen(evalToZero));
		exit(1);
	}
	//actually get the key
	char key[amtChars];
	int i = 0; //start a for loop
	//fill the key
	for (i; i < amtChars; i++) {
		//inspired by ChatGPT
		int randInt = rand() % 27;
		//set spaces
		if (randInt == 26) {
			key[i] = ' ';
		}
		//set letters
		else {
			key[i] = 'A' + randInt;
		}
	}
	write(1, key, amtChars);
	write(1, "\n", 1);
}
