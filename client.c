
/* Author: Colette DeLizo
Date: 28NOV2021
Description: main client funcion for enc_client and dec_client code from assignment example code
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#include <sys/types.h>	// ssize_t
#include <sys/socket.h>	// send(),recv()
#include <netdb.h> 		// gethostbyname()
#include "pads.h"

// Error function used for reporting issues
void error(const char *msg, int value) { 
	perror(msg); 
	exit(value); 
}

int client(char* plaintext, char* key, char* port_arg, char* enc_dec)	//name of input file, name of key, port and decrypt or encrypt
{	
	int i, j;
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[512];
    
	int port = atoi(port_arg);	//convert port string to int

	
	FILE* textFile = fopen(plaintext, "r");			//get text from plaintext
	char text_plaintext[80000];
	fgets(text_plaintext, 80000, textFile);
	fclose(textFile);
	text_plaintext[strcspn(text_plaintext, "\n")] = '\0';	//replace \n with \0

	FILE* keyFile = fopen(key, "r");				//get key text from specified file
	char text_key[80000];
	fgets(text_key, 80000, keyFile);
	fclose(keyFile);
	text_key[strcspn(text_key, "\n")] = '\0';

	int textLength = strlen(text_plaintext);
	int keyLength = strlen(text_key);

	if (keyLength < textLength) {			//check to make sure key is long enough for text file to convert
		fprintf(stderr, "ERROR The key is shorter than the plaintext.\n");
		exit(1);
	}
	
	for (i=0; i<textLength; i++) {				//loop through plain text and compare each character to valid characters in chr[] 
		for (j=0; j<28; j++) {
			if (j == 27) {
				fprintf(stderr, "ERROR %s contains invalid characters.\n", plaintext);
				exit(1);
			}
			if (text_plaintext[i] == chr[j]) {
				break;
			}
		}
	}
	for (i=0; i<keyLength; i++) {		//repeat validation check for key
		for (j=0; j<28; j++) {
			if (j == 27) {
				fprintf(stderr, "Key contains invalid characters.\n");
			}
			if (text_key[i] == chr[j]) {
				break;
			}
		}
	}

// Set up the address struct
		// Clear out the address struct
		memset((char*)&serverAddress, '\0', sizeof(serverAddress));
		// Get the port number, convert to an integer from a string
		portNumber = port;
		// The address should be network capable
		serverAddress.sin_family = AF_INET;
		// Store the port number
		serverAddress.sin_port = htons(portNumber);

		// Get the DNS entry for this host name
		serverHostInfo = gethostbyname("localhost");
		if (serverHostInfo == NULL) {
			fprintf(stderr, "USAGE: ERROR, no such host\n");
			exit(0);
		}
	// Copy the first IP address from the DNS entry to sin_addr.s_addr
		memcpy((char*)&serverAddress.sin_addr.s_addr,
		 (char*)serverHostInfo->h_addr_list[0],
		  serverHostInfo->h_length);

	// Create a socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0) {
		error("CLIENT: ERROR opening socket", 1);
	}
	
	// Connect to server
	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		error("CLIENT: ERROR connecting", 1);

	// Clear out the buffer array
	memset(buffer, '\0', sizeof(buffer));
	if (!strcmp(enc_dec,"enc_client")){
		sprintf(buffer, "%s\n%s\n%s", plaintext, key, "enc_server");//make message, seperated by newline
	}
	else {
		sprintf(buffer, "%s\n%s\n%s", plaintext, key, "dec_server");//make message, seperated by newline
	}

	// Send message to server
 	// Write to the server
	charsWritten = send(socketFD, buffer, strlen(buffer), 0);		//send message
	if (charsWritten < 0) {
		error("CLIENT: ERROR writing to socket",1);
	}
	if (charsWritten < strlen(buffer)){
		printf("CLIENT: WARNIGN: Not all data written to socket!\n");	
	} 

	// Get return message from server
  	// Clear out the buffer again for reuse
	
	memset(buffer, '\0', sizeof(buffer));		//clear buffer					
	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);		//get message
	if (charsRead < 0){
		error("CLIENT: ERROR reading from socket",1);
	}
	
	FILE* recvFile = fopen(buffer, "r");	// Open file returned 
	char output[80000];
	fgets(output, 80000, recvFile);
	fclose(recvFile);

	remove(buffer);	// Delete the file
	printf("%s\n", output);
	 // Close the socket
	close(socketFD);
	return 0;
}