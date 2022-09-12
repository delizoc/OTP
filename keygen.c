
/*
Author:Colette DeLizo
Date:29NOV2021
Decription: Creates a key of given length by user
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "pads.h"

int main (int argc, char* argv[]) {
	
	int j;
	int length = atoi(argv[1]);		//make key the length of given argument input
	char key[length+1];			
	srand(time(0));

	// For each char in key, assign a random value
	for (j=0; j<length; j++) {
		int letter = rand() % 27;
		key[j] = chr[letter];		//use chr[] from pads.h
	}
	key[length] = '\0';	// appended \0 to end
	printf("%s\n", key);
	return 0;
}