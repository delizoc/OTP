/* 
chr[] array with allowable characters, only uppercase alphabet and space (27 total)
initialize encode, decode, client and server functions
*/

#ifndef PADS_H
#define PADS_H

static const char chr[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";      //allowable characters

void encode(char*, char*, char*, int);
void decode(char*, char*, char*, int);

int client(char*, char*, char*, char*);
int server(char*, char*);

#endif
