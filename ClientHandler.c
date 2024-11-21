#include "ClientHandler.h"

int createClientSocket() {
    // Creating a client socket.
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket < 0)
        killProcess("Failed to Create Client Socket\n");
    
    // Connecting to the server.
    struct sockaddr_in servAdd = serverAddress();
    if (connect(clientSocket, (struct sockaddr*)& servAdd, sizeof(servAdd)) < 0)
        killProcess("Failed to Connect to Server\n");

    return clientSocket;
}

void clientList(int clientSocket) {
    // Sending the request header.
    uint8_t* requestBytes = NULL;
    uint32_t requestLength = headerToBytes(1, 0, &requestBytes);
    sendBytes(clientSocket, requestBytes, requestLength);
    free(requestBytes);

    // Receiving the response header.
    uint8_t* responseBytes = NULL;
    uint32_t responseLength = recvBytes(clientSocket, &responseBytes, HEADER_LENGTH);

    // Converting the server's
    // response to a header.    
    Header response = bytesToHeader(responseBytes, responseLength);
    free(responseBytes);

    // Receiving and displaying the output.
    printf("LIST\n");
    uint8_t* output = NULL;
    recvBytes(clientSocket, &output, response.totalBytesToRecv);

    if (strcmp(output, "") == 0) {
        printf(ANSI_COLOR_YELLOW "You currently have no files.\n" ANSI_COLOR_RESET);
    }
    else {
        printf("Your Files:\n");
        printf("%s", output);
    }

    free(output);
}

void clientDiff(char* username, int clientSocket) {
    // Loading the client's library. The
    // username tracks the client's files.
    char* prefix = concatenateStrings(2, "Client_", username);
    Library* library = loadLibrary(prefix);
    free(prefix);

    // Converting the library to bytes.
    uint8_t* libraryBytes = NULL;
    uint32_t libraryLength = libraryToBytes(library, &libraryBytes);
    deleteArray(&library->songs, freeSong);
    free(library);
    
    // Creating the request header. It
    // will indicate the number of bytes
    // (library) it plans to send next.
    uint8_t* requestBytes = NULL;
    uint32_t requestLength = headerToBytes(2, libraryLength, &requestBytes);

    // Sending the bytes and
    // deallocating the bytes.
    sendBytes(clientSocket, requestBytes, requestLength);
    sendBytes(clientSocket, libraryBytes, libraryLength);
    free(requestBytes);
    free(libraryBytes);

    // Receiving the server's response
    // and converting it to a Header.
    uint8_t* responseBytes = NULL;
    uint32_t responseLength = recvBytes(clientSocket, &responseBytes, HEADER_LENGTH);
    Header response = bytesToHeader(responseBytes, responseLength);
    free(responseBytes);

    // Receiving the output, and
    // displaying the output.
    printf("DIFF\n");
    uint8_t* output = NULL;
    recvBytes(clientSocket, &output, response.totalBytesToRecv);

    if (strcmp(output, "") == 0) {
        printf(ANSI_COLOR_YELLOW "There are no files to compare.\n" ANSI_COLOR_RESET);
    }
    else {
        printf("The '+' means that you currently have this file.\n");
        printf("The '-' means that you don't currently have this file.\n");
        printf("%s", output);
    }

    free(output);
}

void clientPull(char* username, int clientSocket) {
    // Sending the request header.
    uint8_t* requestBytes = NULL;
    uint32_t requestLength = headerToBytes(3, 0, &requestBytes);
    sendBytes(clientSocket, requestBytes, requestLength);
    free(requestBytes);

    // Receiving the server's response
    // and converting it to a Header.
    uint8_t* responseBytes = NULL;
    uint32_t responseLength = recvBytes(clientSocket, &responseBytes, HEADER_LENGTH);
    Header response = bytesToHeader(responseBytes, responseLength);
    free(responseBytes);
  
    printf("PULL\n");
    uint8_t* output = NULL;
    uint32_t outputLength = recvBytes(clientSocket, &output, response.totalBytesToRecv);

    // If there are no bytes to receive,
    // there are no files to pull.
    if (response.totalBytesToRecv == 0) {
        printf(ANSI_COLOR_YELLOW "There are no files to pull.\n" ANSI_COLOR_RESET);
    }
    // Downloading the files.
    else {
        printf("Pulling Files...\n");
        downloadFileList(output, outputLength, username);
        printf("Pulled Files.\n");
    }

    free(output);
}

void clientLeave(int clientSocket) {
    // Notifying the server that we're leaving.
    uint8_t* requestBytes = NULL;
    uint32_t requestLength = headerToBytes(4, 0, &requestBytes);
    sendBytes(clientSocket, requestBytes, requestLength);
    free(requestBytes);
}