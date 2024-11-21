#include "Song.h"

Song* loadSong(char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL)
        killProcess("Failed to Open File\n");
    
    size_t zeroLength = 0;

    char* ISWC = NULL;
    int bytesRead = getline(&ISWC, &zeroLength, file);
    if (bytesRead == -1)
        killProcess("Failed to Read File\n");
    ISWC[bytesRead-1] = '\0';

    char* title = NULL;
    bytesRead = getline(&title, &zeroLength, file);
    if (bytesRead == -1)
        killProcess("Failed to Read File\n");
    title[bytesRead-1] = '\0';

    fclose(file);
    Song* song = malloc(sizeof(Song));
    if (song == NULL)
        killProcess(MALLOC_ERROR);
    song->ISWC = ISWC;
    song->title = title;
    return song;
}

Library* loadLibrary(char* prefix) {
    DIR* dir = opendir(".");
    struct dirent *entry;
    
    Library* library = malloc(sizeof(Library));
    if (library == NULL)
        killProcess(MALLOC_ERROR);
    library->songs = createArray(sizeof(Song*));

    Song* uniqueSongs = NULL; // Hash table for unique songs (no content)

    while (entry = readdir(dir)) {
        char* filename = entry->d_name;

        // Checking if File Matches Prefix
        if (strlen(filename) < strlen(prefix))
            continue;
        if (strncmp(filename, prefix, strlen(prefix)) != 0)
            continue;

        // Loading Song
        Song* song = loadSong(filename);

         Song* s;
        HASH_FIND_STR(uniqueSongs, song->ISWC, s);
        if (s == NULL) {
            // This is a new unique song
            HASH_ADD_KEYPTR(hh, uniqueSongs, song->ISWC, strlen(song->ISWC), song);
            // Add to library
            addElement(&library->songs, (void*)&song);
        } else {
            // This is a duplicate, free the memory
            freeSong(&song);
        }
    }

    closedir(dir);
    return library;
}

uint32_t songToBytes(Song* song, uint8_t** bytes) {
    // This is the order of the values written to the bytes:
    // ISWC Length -> Title Length -> ISWC Bytes -> Title Bytes
    uint32_t iswcLength = strlen(song->ISWC) + 1;
    uint32_t titleLength = strlen(song->title) + 1;

    uint32_t numberBytes = 2 * UINT32_T_SIZE + iswcLength + titleLength;
    *bytes = malloc(numberBytes);
    if (*bytes == NULL)
        killProcess("Failed to Allocate Memory\n");

    unsigned int offset = 0;
    memcpy(*bytes + offset, &iswcLength, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;

    memcpy(*bytes + offset, &titleLength, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;

    memcpy(*bytes + offset, song->ISWC, iswcLength);
    offset += iswcLength;

    memcpy(*bytes + offset, song->title, titleLength);
    offset += titleLength;

    return numberBytes;
}

Song* bytesToSong(uint8_t* bytes, uint32_t numberBytes) {
    Song* song = malloc(sizeof(Song));
    if (song == NULL)
        killProcess(MALLOC_ERROR);
    unsigned int offset = 0;

    uint32_t iswcLength = 0;
    memcpy(&iswcLength, bytes + offset, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;

    uint32_t titleLength = 0;
    memcpy(&titleLength, bytes + offset, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;

    char* ISWC = malloc(iswcLength);
    if (ISWC == NULL)
        killProcess(MALLOC_ERROR);
    memcpy(ISWC, bytes + offset, iswcLength);
    offset += iswcLength;

    char* title = malloc(titleLength);
    if (title == NULL)
        killProcess(MALLOC_ERROR);
    memcpy(title, bytes + offset, titleLength);
    offset += titleLength;

    song->ISWC = ISWC;
    song->title = title;
    
    return song;
}

uint32_t libraryToBytes(Library* library, uint8_t** bytes) {
    // This is the order of the values written to the bytes:
    // # of Songs -> Number of Bytes in Song 1 -> Bytes in Song 1 -> ... -> Number of Bytes in Song N -> Bytes in Song N
    unsigned int numberSongs = library->songs->arrayLength;
    
    Array* bytesInSong = createArray(UINT8_T_PTR_SIZE);
    Array* bytesPerSong = createArray(UINT32_T_PTR_SIZE);

    // Calculating the Total # of Bytes
    uint32_t totalNumberBytes = UINT32_T_SIZE * (numberSongs + 1);

    for (unsigned int i = 0; i < numberSongs; i++) {
        uint8_t* sBytes = NULL;

        uint32_t* sNumberBytes = malloc(UINT32_T_SIZE);
        if (sNumberBytes == NULL)
            killProcess(MALLOC_ERROR);

        Song** song = (Song**) getElement(&library->songs, i);
        if (*song == NULL)
            killProcess(INDEXING_ERROR);
        *sNumberBytes = songToBytes(*song, &sBytes);

        addElement(&bytesInSong, &sBytes);
        addElement(&bytesPerSong, &sNumberBytes);

        totalNumberBytes += *sNumberBytes;
    }
    
    // Initializing the Byte Array
    // Writing the Number of Songs
    *bytes = malloc(totalNumberBytes);
    if (*bytes == NULL)
        killProcess(MALLOC_ERROR);
    memcpy(*bytes, &numberSongs, UINT32_T_SIZE);

    // Encoding the Library
    unsigned int offset = UINT32_T_SIZE;
    for (unsigned int i = 0; i < numberSongs; i++) {
        uint8_t** sBytes = (uint8_t**) getElement(&bytesInSong, i);
        uint32_t** sNumberBytes = (uint32_t**) getElement(&bytesPerSong, i);

        memcpy(*bytes + offset, *sNumberBytes, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;

        memcpy(*bytes + offset, *sBytes, **sNumberBytes);
        offset += **sNumberBytes;
    }

    deleteArray(&bytesInSong, freeUINT8_T);
    deleteArray(&bytesPerSong, freeUINT32_T);

    return totalNumberBytes;
}

Library* bytesToLibrary(uint8_t* bytes, uint32_t numberBytes) {
    uint32_t numberSongs = 0;
    memcpy(&numberSongs, bytes, UINT32_T_SIZE);

    Library* library = malloc(sizeof(Library));
    if (library == NULL)
        killProcess(MALLOC_ERROR);
    library->songs = createArray(sizeof(Song*));

    unsigned int offset = UINT32_T_SIZE;
    for (unsigned int i = 0; i < numberSongs; i++) {
        uint32_t sNumberBytes = 0;
        memcpy(&sNumberBytes, bytes + offset, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;

        uint8_t* sBytes = malloc(sNumberBytes);
        if (sBytes == NULL)
            killProcess(MALLOC_ERROR);
        memcpy(sBytes, bytes + offset, sNumberBytes);
        offset += sNumberBytes;

        Song* song = bytesToSong(sBytes, sNumberBytes);
        addElement(&library->songs, (void*)& song);

        free(sBytes);
    }

    return library;
}

char* compareLibraries(Library* serverLibrary, Library* clientLibrary, Array** diff) {
    // This will store the titles associated with an ISWC.
    Map* iswcToTitle = createMap(CHAR_PTR_SIZE);

    // Loading the Server's ISWCs into an Array
    Array* serverISWC = createArray(CHAR_PTR_SIZE);
    for (unsigned int i = 0; i < serverLibrary->songs->arrayLength; i++) {
        Song** song = (Song**) getElement(&serverLibrary->songs, i);
        if (*song == NULL)
            killProcess(INDEXING_ERROR);

        addElement(&serverISWC, (void*)& (*song)->ISWC);
        putEntry(&iswcToTitle, (*song)->ISWC, (*song)->title);
    }

    // Loading the Client's ISWCs into an Array
    Array* clientISWC = createArray(CHAR_PTR_SIZE);
    for (unsigned int i = 0; i < clientLibrary->songs->arrayLength; i++) {
        Song** song = (Song**) getElement(&clientLibrary->songs, i);
        if (*song == NULL)
            killProcess(INDEXING_ERROR);

        addElement(&clientISWC, (void*)& (*song)->ISWC);
        putEntry(&iswcToTitle, (*song)->ISWC, (*song)->title);
    }

    // Sorting to Speed up Searching
    qsort(serverISWC->array, serverISWC->arrayLength, serverISWC->elementSize, __strcmp);
    qsort(clientISWC->array, clientISWC->arrayLength, clientISWC->elementSize, __strcmp);

    char* output = "";
    for (unsigned int i = 0; i < serverISWC->arrayLength; i++) {
        // We're iterating through the server's ISWCs,
        // checking to see if there's any overlap with the client.
        char** ISWC = (char**) getElement(&serverISWC, i);
        if (*ISWC == NULL)
            killProcess(INDEXING_ERROR);
        
        char* found = (char*) bsearch(ISWC, clientISWC->array, clientISWC->arrayLength, clientISWC->elementSize, __strcmp);
        
        // The server and the client both have said song.
        if (found != NULL) {
            char* title = (char*) getEntry(&iswcToTitle, *ISWC);
            if (title == NULL)
                killProcess(INDEXING_ERROR);
            
            char* _output = concatenateStrings(6, output, ANSI_COLOR_GREEN, "+ ", title, "\n", ANSI_COLOR_RESET);
            if (strcmp(output, "") != 0)
                free(output);
            output = _output;
        }
        // The client does not have said song.
        // We need to keep track of the ISWC for the PULL operation.
        else {
            char* title = (char*) getEntry(&iswcToTitle, *ISWC);
            if (title == NULL)
                killProcess(INDEXING_ERROR);

            char* _output = concatenateStrings(6, output, ANSI_COLOR_RED, "- ", title, "\n", ANSI_COLOR_RESET);
            if (strcmp(output, "") != 0)
                free(output);
            output = _output;

            // Need to make a copy of the ISWC to prevent
            // memory errors.
            char* _ISWC = malloc(strlen(*ISWC) + 1);
            strcpy(_ISWC, *ISWC);
            _ISWC[strlen(*ISWC)] = '\0';
            addElement(diff, &_ISWC);
        }
    }

    deleteMap(&iswcToTitle, NULL);
    deleteArray(&serverISWC, NULL);
    deleteArray(&clientISWC, NULL);
    return output;
}

void freeSong(void* song) {
    Song** _song = (Song**) song;
    free((*_song)->ISWC);
    free((*_song)->title);
    free(*_song);
}