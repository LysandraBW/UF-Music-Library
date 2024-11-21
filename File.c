#include "File.h"

uint32_t fileToBytes(char* filepath, uint8_t** bytes) {
    FILE* file;
    file = fopen(filepath, "r");
    if (file == NULL)
        killProcess("Failed to Open File\n");
    
    unsigned int index = 0;
    uint32_t size = 0;

    // We read each character from the file
    // into the byte array.
    while (1) {
        char ch = fgetc(file);
        if (ch == EOF)
            break;

        *bytes = realloc(*bytes, ++size * CHAR_SIZE);
        if (*bytes == NULL)
            killProcess(MALLOC_ERROR);
        (*bytes)[index++] = ch;
    }

    fclose(file);
    return size;
}

uint32_t fileListToBytes(Array* filepaths, uint8_t** bytes) {
    // Stores the byte arrays for each of the files.
    Array* bytesInFile = createArray(UINT8_T_PTR_SIZE);

    // Stores the size of the byte arrays for each of the files.
    Array* bytesPerFile = createArray(UINT32_T_PTR_SIZE);

    // totalNumberBytes represents the total number of bytes that we'll
    // need to encode all the files, and it will be used to initialize
    // the buffer in which we encode the files in.
    uint32_t numberFiles = filepaths->arrayLength;
    uint32_t totalNumberBytes = UINT32_T_SIZE * (numberFiles + 1);

    // In this loop, we're storing information about
    // each file in the two Array objects.
    for (unsigned int i = 0; i < numberFiles; i++) {
        uint8_t* fBytes = NULL;
        
        uint32_t* fNumberBytes = malloc(sizeof(UINT32_T_SIZE));
        if (fNumberBytes == NULL)
            killProcess(MALLOC_ERROR);
        
        char** fp = (char**) getElement(&filepaths, i);
        if (*fp == NULL)
            killProcess(INDEXING_ERROR);

        *fNumberBytes = fileToBytes(*fp, &fBytes);

        // Storing information about the encoded file.
        addElement(&bytesInFile, &fBytes);
        addElement(&bytesPerFile, &fNumberBytes);

        // Updating the total number of bytes.
        totalNumberBytes += *fNumberBytes;
    }

    // Initializing the bytes that we'll encode the files in,
    // and writing the number of files to said bytes.
    *bytes = malloc(totalNumberBytes);
    if (*bytes == NULL)
        killProcess(MALLOC_ERROR);
    memcpy(*bytes, &numberFiles, UINT32_T_SIZE);

    // Writing each of the files to the byte array. 
    // The size of that file is written first.
    // Then, the actual bytes are written.
    unsigned int offset = UINT32_T_SIZE;
    for (unsigned int i = 0; i < numberFiles; i++) {
        uint8_t** fBytes = (uint8_t**) getElement(&bytesInFile, i);
        if (*fBytes == NULL)
            killProcess(INDEXING_ERROR);

        uint32_t** fNumberBytes = (uint32_t**) getElement(&bytesPerFile, i);
        if (*fNumberBytes == NULL)
            killProcess(INDEXING_ERROR);

        memcpy(*bytes + offset, *fNumberBytes, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;

        memcpy(*bytes + offset, *fBytes, **fNumberBytes);
        offset += **fNumberBytes;
    }

    deleteArray(&bytesInFile, freeUINT8_T);
    deleteArray(&bytesPerFile, freeUINT32_T);

    return totalNumberBytes;
}

void downloadFile(uint8_t* bytes, uint32_t numberBytes, char* username) {
    // To get the filename,
    // we need the ISWC. Luckily,
    // it's the first 13 characters
    // of the file (bytes).
    char ISWC[14];
    strncpy(ISWC, bytes, 13);
    ISWC[13] = '\0';

    char* filename = concatenateStrings(5, "Client_", username, "_", ISWC, ".txt");
    printf("\tDownloading File '%s'...\n", filename);

    // Opening the File
    FILE* file;
    file = fopen(filename, "w");
    if (file == NULL)
        killProcess("Failed to Create New File\n");
    
    // Likely a very expensive operation, to read character by character.
    // But, I'm not sure of another way that's not difficult.
    for (unsigned int i = 0; i < numberBytes; i++)
        fputc(bytes[i], file);
    
    fclose(file);
    free(filename);
}

void downloadFileList(uint8_t* bytes, uint32_t numberBytes, char* username) {
    // Extracting the number of files from the bytes.
    uint32_t numberFiles = 0;
    memcpy(&numberFiles, bytes, UINT32_T_SIZE);

    unsigned int offset = UINT32_T_SIZE;
    for (unsigned int i = 0; i < numberFiles; i++) {
        uint32_t fNumberBytes = 0;
        memcpy(&fNumberBytes, bytes + offset, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;

        uint8_t* fBytes = malloc(fNumberBytes);
        if (fBytes == NULL)
            killProcess(MALLOC_ERROR);
        
        memcpy(fBytes, bytes + offset, fNumberBytes);
        offset += fNumberBytes;

        downloadFile(fBytes, fNumberBytes, username);
        free(fBytes);
    }
}

Array* clientFileList(char* clientID, Array* ISWCs) {
    Array* filepaths = createArray(CHAR_PTR_SIZE);
    for (unsigned int i = 0; i < ISWCs->arrayLength; i++) {
        char** ISWC = (char**) getElement(&ISWCs, i);
        if (*ISWC == NULL)
            killProcess(INDEXING_ERROR);

        // We're just taking two parts and making them whole.
        // Also, this is the client files of the server, which
        // is why we're prefixing the filepath with a "Server_".
        char* filepath = concatenateStrings(5, "Server_", clientID, "_", *ISWC, ".txt");
        addElement(&filepaths, &filepath);
        // free(filepath); // UNSURE
    }
    return filepaths;
}