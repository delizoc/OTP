/*
Author: Colette DeLizo
Date: 29NOV2021
Description: checks arguments then calls tells server to decode in server.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pads.h"

int main(int argc, char *argv[]) {

	// Check arguments
	if (argc < 2) {
		fprintf(stderr,"USAGE: %s port\n", argv[0]);
		exit(1);
	}
	// Call server() tell it to decode
	server(argv[1], "dec_server");

	return 0;
}