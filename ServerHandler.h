#ifndef SERVER_HANDLER
#define SERVER_HANDLER

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Map.h"
#include "Host.h"
#include "Utils.h"
#include "Song.h"
#include "File.h"

/*
This file contains the main functions that the server will use.
*/

/* 
Wraps all the data the server 
may need to execute a function.
*/
typedef struct {
    Map* diffs;
    Header* header;
    char* clientID;
    int clientSocket;
    int serverSocket;
} ActionData;

/*
These functions concern themselves
with defining a part of the server.
*/
int createServerSocket();
struct sockaddr_in serverAddress();

/*
These functions concern themselves
with the server's functionality.
*/
void serverList(ActionData** data);
void serverDiff(ActionData** data);
void serverPull(ActionData** data);

/*
This function will call the function
specified in the header.
*/
void handleClient(ActionData** data);

#endif