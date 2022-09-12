/*
Author: Colette DeLizo
Date: 29NOV2021
Description: checks arguments calls client() function in client.c and tells it to encode
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pads.h"

int main(int argc, char *argv[]) {

	// Check arguments
	if (argc < 4) {
		fprintf(stderr,"USAGE: %s plaintext key port\n", argv[0]);
		exit(0);
	}
	// client encode
	client(argv[1], argv[2], argv[3], "enc_client");

	return 0;
}
