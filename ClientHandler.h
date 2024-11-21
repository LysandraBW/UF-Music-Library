#ifndef CLIENT_HANDLER
#define CLIENT_HANDLER

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ServerHandler.h"
#include "Utils.h"
#include "Macros.h"
#include "Song.h"

/*
This file contains the main functions that the client will use.
*/

int createClientSocket();

void clientList(int clientSocket);
void clientDiff(char* username, int clientSocket);
void clientPull(char* username, int clientSocket);
void clientLeave(int clientSocket);

#endif