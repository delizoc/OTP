//Encode and decode functions

#include <stdio.h>
#include <string.h>
#include "pads.h"

void characterInteger(char in[], int out[], int size) {	//charcter to an integer 
	int i, j;
	for (i=0; i<size; i++) {
		for (j=0; j<27; j++) {
			if (in[i] == chr[j]) {		//based on chr[] in pads.h
				out[i] = j;
				break;
			}
		}
	}
}
void integerCharacter(int in[], char out[], int size) {	// integer to a character  
	int i;
	for (i=0; i<size; i++) {
		out[i] = chr[in[i]];			//based on chr[] in pads.h
	}
}

// encoded an input string using key, output it encoded string size is the length of the string.
void encode(char key[], char input[], char output[], int size) {
	int i;

	// Convert input array to integers
	int intIn[size];
	characterInteger(input, intIn, size);

	// Convert key to integers
	int intKey[size];
	characterInteger(key, intKey, size);

	// Generate output in integer form
	int intOut[size];
	for (i=0; i<size; i++) {
		intOut[i] = (intIn[i] + intKey[i]) % 27;
	}

	// Convert output to characters
	integerCharacter(intOut, output, size);
	output[size] = '\0';
}

//decodes string that has previously been encoded, uses key with input key and output string is decoded.
void decode(char key[], char input[], char output[], int size) {
	int i;

	// Convert input array to integers
	int intIn[size];
	characterInteger(input, intIn, size);

	// Convert key to integers
	int intKey[size];
	characterInteger(key, intKey, size);

	// Generate output in integer form
	int intOut[size];
	for (i=0; i<size; i++) {
		intOut[i] = intIn[i] - intKey[i];
		if (intOut[i] < 0) intOut[i] += 27;
	}

	// Convert output to characters
	integerCharacter(intOut, output, size);
	output[size] = '\0';
}
