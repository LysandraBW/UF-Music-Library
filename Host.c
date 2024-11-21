#include "Host.h"

uint32_t headerToBytes(uint32_t action, uint32_t totalBytesToRecv, uint8_t** bytes) {
    unsigned int numberBytes = 2 * UINT32_T_SIZE;
    *bytes = malloc(numberBytes);
    if (*bytes == NULL)
        killProcess(MALLOC_ERROR);
    memcpy(*bytes, &action, UINT32_T_SIZE);
    memcpy(*bytes + UINT32_T_SIZE, &totalBytesToRecv, UINT32_T_SIZE);
    return numberBytes;
}

Header bytesToHeader(uint8_t* bytes, uint32_t numberBytes) {
    Header header;
    memcpy(&header.action, bytes, UINT32_T_SIZE);
    memcpy(&header.totalBytesToRecv, bytes + UINT32_T_SIZE, UINT32_T_SIZE);
    return header;
}

uint32_t stringToBytes(char* str, uint8_t** bytes) {
    uint32_t numberBytes = strlen(str) + 1;
    *bytes = malloc(numberBytes);
    if (*bytes == NULL)
        killProcess(MALLOC_ERROR);
    strcpy(*bytes, str);
    (*bytes)[numberBytes-1] = '\0';
    return numberBytes;
}

char* bytesToString(uint8_t* bytes, uint32_t numberBytes) {
    char* string = malloc(numberBytes);
    if (string == NULL)
        killProcess(MALLOC_ERROR);
    strcpy(string, bytes);
    string[numberBytes-1] = '\0';
    return string;
}

void sendBytes(int clientSocket, uint8_t* bytes, uint32_t numberBytes) {
    int numberBytesSent = send(clientSocket, bytes, numberBytes, 0);
    if (numberBytesSent != numberBytes)
        killProcess("Failed to Send Bytes\n");
}

unsigned int recvBytes(int clientSocket, uint8_t** bytes, uint32_t numberBytes) {
    *bytes = malloc(numberBytes);
    if (*bytes == NULL)
        killProcess(MALLOC_ERROR);
    
    unsigned int totalBytesRcvd = 0;
    while (totalBytesRcvd < numberBytes) {
        unsigned int numberBytesRcvd = recv(clientSocket, *bytes + totalBytesRcvd, numberBytes - totalBytesRcvd, 0);
        if (numberBytesRcvd == -1)
            killProcess("Error Occurred While Receiving Bytes\n");
        if (numberBytesRcvd == 0)
            break;
        totalBytesRcvd += numberBytesRcvd;
    }

    return totalBytesRcvd;
}