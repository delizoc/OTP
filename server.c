/*
Author: Colette DeLizo
Date: 28NOV2021
Description: main server file for enc_server and dec_server code from example assignemnt for sockets
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "pads.h"

// Error function
void error(const char *msg) { 
	perror(msg); 
	exit(1); 
	}

int server(char* port_arg, char* enc_dec)		//port as string and if running encrypt or decrypt
{
	int wrongFile = 0;		
	int listenSocket, connectionSocket, portNumber, charsRead, status;
	socklen_t sizeOfClientInfo;
	char buffer[512];
	char inputFileName[256];
	char keyFileName[256];
	char inputText[80000];
	char key[80000];
	char outputText[80000];
	struct sockaddr_in serverAddress, clientAddress;
	pid_t pid, sid;

	// Set up the address struct for the server
		// Clear out the address struct
		memset((char *)&serverAddress, '\0', sizeof(serverAddress));
		// Get the port number, convert to an integer from a string
		portNumber = atoi(port_arg);
		// The address should be network capable
		serverAddress.sin_family = AF_INET;
		// Store the port number
		serverAddress.sin_port = htons(portNumber);
		// Allow a client at any address to connect to this server
		serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Create the socket that will listen for connections
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket < 0){
		error("ERROR opening socket");
	} 

	// Associate the socket to the port
	if (bind(listenSocket, 
		(struct sockaddr *)&serverAddress, 
		sizeof(serverAddress)) < 0) 
		error("ERROR on binding");

	// Start listening for connetions. Allow up to 5 connections to queue up
	listen(listenSocket, 5);

	while(1) {
		// Get the size of the address for the client that will connect
		sizeOfClientInfo = sizeof(clientAddress);
		// Accept the connection request which creates a connection socket
		connectionSocket = accept(listenSocket, 
			(struct sockaddr *)&clientAddress,
			&sizeOfClientInfo);
		if (connectionSocket < 0) error("ERROR on accept");
		//fork it!!
		pid = fork();
		switch (pid) {
			case -1: ;
				error("ERROR creating fork");
				exit(0);
				break;
			case 0: ;
				// Get the message from the client and display it
				memset(buffer, '\0', sizeof(buffer));
				// Read the client's message from the socket
				charsRead = recv(connectionSocket, buffer, sizeof(buffer)-1, 0);
				if (charsRead < 0) error("ERROR reading from socket");
				const char newline[2] = {'\n', '\0'};		//used for token
				
				char *token = strtok(buffer, newline);	//plaintext
				strcpy(inputFileName, token);
				
				token = strtok(NULL, newline);		//key
				strcpy(keyFileName, token);
				
				token = strtok(NULL, newline);		// Make sure the right program is connecting
				if (strcmp(enc_dec, token)) {
					fprintf(stderr, "ERROR %s cannot use %s.\n", "dec_client", enc_dec );		//should use token but changed all tokens to _server not client
					wrongFile = 1;
				}

				if (!wrongFile){
					// Open the input and key and put them into strings
					// Remove newlines at end
					FILE* inputFile = fopen(inputFileName, "r");
					fgets(inputText, 80000, inputFile);
					fclose(inputFile);
					inputText[strcspn(inputText, "\n")] = '\0';

					FILE* keyFile = fopen(keyFileName, "r");
					fgets(key, 80000, keyFile);
					fclose(keyFile);
					key[strcspn(key, "\n")] = '\0';
					
					if (!strcmp(enc_dec, "enc_server")) {			//encode
						encode(key, inputText, outputText, strlen(inputText));
					}
					else if (!strcmp(enc_dec, "dec_server")) {		//decode
						decode(key, inputText, outputText, strlen(inputText));
					}
				}	
				// Write outputText to unique file
				// Create a unique file name
				int uniquePid = getpid();
				char uniqueFile[32];
				sprintf(uniqueFile, "%s_f.%d", enc_dec, uniquePid);

				// Open it
				// Write the output text to it
				// Write nothing if it is the wrong program connecting
				FILE* uniqueFD = fopen(uniqueFile, "w+");
				if (wrongFile) {
					fprintf(uniqueFD, "");
				} else {
					fprintf(uniqueFD, "%s", outputText);
				}
				fclose(uniqueFD);

				// Send a Success message back to the client
				charsRead = send(connectionSocket, uniqueFile, strlen(uniqueFile), 0);
				if (charsRead < 0) error("ERROR writing to socket");
				
				// Close the connection socket for this client
				close(connectionSocket);
				connectionSocket = -1;
				exit(0);
				break;
			default: ;
		}
				// wait for zombie programs and kill them
				close(connectionSocket);
				connectionSocket = -1;
				wait(NULL);
	}
	// Close the listening socket
	close(listenSocket);
	return 0;
}
