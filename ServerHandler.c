#include "ServerHandler.h"

int createServerSocket() {
    // Creating the socket.
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0)
        killProcess("Server Socket Error\n");

    // Binding the socket.
    struct sockaddr_in address = serverAddress();
    if (bind(serverSocket, (struct sockaddr*)& address, sizeof(address)) < 0)
        killProcess("Failed to Bind Server Socket\n");

    // Listens to 100 connection.
    if (listen(serverSocket, 100) < 0)
        killProcess("Server Socket Failed to Listen\n");

    return serverSocket;
}

struct sockaddr_in serverAddress() {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9000);
    return address;
}

void serverList(ActionData** data) {
    // The prefix is used to search for a client's files.
    char* prefix = concatenateStrings(3, "Server_", (*data)->clientID, "_");
    Library* library = loadLibrary(prefix);
    free(prefix);

    // Adding the song's to the output.
    char* output = "";
    for (unsigned int i = 0; i < library->songs->arrayLength; i++) {
        Song** song = (Song**) getElement(&library->songs, i);
        if (*song == NULL)
            killProcess(INDEXING_ERROR);
        
        char* _output = concatenateStrings(5, output, (*song)->title, " [", (*song)->ISWC, "]\n");
        if (strcmp(output, "") != 0)
            free(output);
        output = _output;
    }
    
    // Converting the output to bytes.
    uint8_t* outputBytes = NULL;
    uint32_t outputLength = stringToBytes(output, &outputBytes);

    // Converting the header to bytes.
    uint8_t* headerBytes = NULL;
    uint32_t headerLength = headerToBytes(1, outputLength, &headerBytes);

    // Sending the bytes to the client.
    sendBytes((*data)->clientSocket, headerBytes, headerLength);
    sendBytes((*data)->clientSocket, outputBytes, outputLength);

    // Deallocating any memory that we don't need.
    if (strcmp(output, "") != 0)
        free(output);

    free(headerBytes);
    free(outputBytes);
    deleteArray(&library->songs, freeSong);
    free(library);
}

void serverDiff(ActionData** data) {
    // The prefix is used to search for a client's files.
    char* prefix = concatenateStrings(3, "Server_", (*data)->clientID, "_");
    Library* serverLibrary = loadLibrary(prefix);
    free(prefix);

    // Receiving the bytes of the client's library,
    // and converting them into the actual Library object.
    uint8_t* clientLibraryBytes = NULL;
    uint32_t clientLibraryBytesRcvd = recvBytes((*data)->clientSocket, &clientLibraryBytes, (*data)->header->totalBytesToRecv);

    if (clientLibraryBytesRcvd != (*data)->header->totalBytesToRecv)
        killProcess("Failed to Receive All Bytes\n");
    
    Library* clientLibrary = bytesToLibrary(clientLibraryBytes, clientLibraryBytesRcvd);
    free(clientLibraryBytes);

    // This will store the ISWCs that
    // the client doesn't have.
    Array* diff = createArray(CHAR_PTR_SIZE);
    if (diff == NULL)
        killProcess(MALLOC_ERROR);

    // Logging the output, and storing the differences in a Map object.
    char* output = compareLibraries(serverLibrary, clientLibrary, &diff);
    putEntry(&((*data)->diffs), (*data)->clientID, (void*) diff);

    // Converting the output into bytes.
    uint8_t* outputBytes = NULL;
    uint32_t outputLength = stringToBytes(output, &outputBytes);

    // Converting the header into bytes.
    uint8_t* headerBytes = NULL;
    uint32_t headerLength = headerToBytes(2, outputLength, &headerBytes);

    // Sending the header and output.
    sendBytes((*data)->clientSocket, headerBytes, headerLength);
    sendBytes((*data)->clientSocket, outputBytes, outputLength);

    // Deallocating now unused memory.
    if (strcmp(output, "") != 0)
        free(output);

    free(headerBytes);
    free(outputBytes);
    deleteArray(&clientLibrary->songs, freeSong);
    free(clientLibrary);
    deleteArray(&serverLibrary->songs, freeSong);
    free(serverLibrary);
}

void serverPull(ActionData** data) {
    // Retrieving the ISWCs of the
    // songs that the client doesn't have.
    Array* ISWCs = (Array*) getEntry(&((*data)->diffs), (*data)->clientID);
    int canPull = ISWCs != NULL && ISWCs->arrayLength != 0 ? 1 : 0;
    
    // If the client didn't DIFF before a PULL,
    // ISWCs will be NULL. So, to prevent any
    // errors, we're only going to load the filepaths
    // if ISWCs is not NULL.
    Array* filepaths = NULL;
    if (canPull) 
        filepaths = clientFileList((*data)->clientID, ISWCs);

    uint8_t* fileListBytes = NULL;
    uint32_t fileListNumberBytes = 0;
    
    // For the same reason as before,
    // we'll only call this function
    // if there's something to actually send.
    if (canPull)
        fileListNumberBytes = fileListToBytes(filepaths, &fileListBytes);

    uint8_t* headerBytes = NULL;
    uint32_t headerNumberBytes = headerToBytes(3, fileListNumberBytes, &headerBytes);
    sendBytes((*data)->clientSocket, headerBytes, headerNumberBytes);
    
    // We'll only be sending a file list
    // if there's something to send.
    if (canPull)
        sendBytes((*data)->clientSocket, fileListBytes, fileListNumberBytes);

    free(headerBytes);
    if (fileListBytes != NULL)
        free(fileListBytes);
    if (filepaths != NULL)
        deleteArray(&filepaths, freeString);
}

void handleClient(ActionData** data) {
    switch ((*data)->header->action) {
        case 1:
            serverList(data);
            break;
        case 2:
            serverDiff(data);
            break;
        case 3:
            serverPull(data);
            break;
        default:
            break;
    }
}