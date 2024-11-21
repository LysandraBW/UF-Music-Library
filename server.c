#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "ServerHandler.h"
#include "ClientHandler.h"
#include "Map.h"
#include "Host.h"
#include "Utils.h"
#include "Macros.h"

void* handleThread(void* _data);

int main() {
    int serverSocket = createServerSocket();

    // Stores all the differences between a
    // server and its various clients.
    Map* diffs = createMap(sizeof(Array*));

    while (1) {
        // Connecting to Client
        // The address and address_len are NULL because
        // we're not interested in the client's address.
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket < 0)
            killProcess("Failed to Accept Client Socket\n");

        // Receiving Username
        uint8_t* username = NULL;
        uint32_t usernameLength = recvBytes(clientSocket, &username, USERNAME_LENGTH);

        // Data for a Thread
        ActionData* data = malloc(sizeof(ActionData));
        data->clientID = (char*) username;
        data->clientSocket = clientSocket;
        data->serverSocket = serverSocket;
        data->diffs = diffs;
        data->header = NULL;

        // Creating Thread
        pthread_t thread;
        if (pthread_create(&thread, NULL, handleThread, (void*) data) != 0)
            killProcess("Failed to Create Thread");
    }
}

void* handleThread(void* _data) {
    pthread_detach(pthread_self());
    ActionData* data = (ActionData*) _data;

    while (1) {
        // Receiving Client's Request
        uint8_t* requestHeaderBytes = NULL;
        uint32_t requestHeaderLength = recvBytes(data->clientSocket, &requestHeaderBytes, HEADER_LENGTH);
        if (requestHeaderLength != HEADER_LENGTH)
            killProcess("Failed to Receive Header\n");

        // Turning Bytes into Header
        Header* header = malloc(sizeof(Header));
        if (header == NULL)
            killProcess("Failed to Allocate Memory\n");
        *header = bytesToHeader(requestHeaderBytes, HEADER_LENGTH);
        data->header = header;
        free(requestHeaderBytes);

        // Client is Leaving
        if (data->header->action == 4) {
            free(header);
            free(data);
            return NULL;
        }

        // Handle Request
        handleClient(&data);
        free(header);
    }
}