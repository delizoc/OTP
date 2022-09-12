/*
Author: Colette DeLizo
Date: 29NOV2021
Description: checks arguments then calls client in client.c 
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
	//client() decode
	client(argv[1], argv[2], argv[3], "dec_client");
	return 0;
}
