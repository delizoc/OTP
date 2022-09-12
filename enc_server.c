/*
Author: Colette DeLizo
Date: 29NOV2021
Description: Calls server function in server.c, and tells it encode the given file argv[1]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pads.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {			//check to make sure valid input
		fprintf(stderr,"USAGE: %s port\n", argv[0]);
		exit(1);
	}
	server(argv[1], "enc_server");	//tells server to encode
	return 0;
}