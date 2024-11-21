#include "Utils.h"

void killProcess(char* str) {
    printf("%s", str);
    exit(-1);
}

char* concatenateStrings(unsigned int n, ...) {
    if (n == 0)
        return "";

    va_list ptr;
    va_start(ptr, n);

    // First Argument
    // The 1st argument is used to initialize
    // the buffer.
    char* string = va_arg(ptr, char*);

    // Initializing the Buffer
    int length = strlen(string) + 1;
    char* buffer = malloc(length);
    buffer[0] = '\0';

    // Copying the String
    strcat(buffer, string);

    // Repeating the process for the other arguments,
    // except we resize the buffer instead of initializing it.
    // It might be better to figure out the total length,
    // instead of constantly resizing it.
    for (int i = 1; i < n; i++) {
        string = va_arg(ptr, char*);
        length += strlen(string);

        char* _buffer = realloc(buffer, length);
        if (_buffer != NULL)
            buffer = _buffer;
        else
            killProcess("Failed to Resize Buffer\n");

        strcat(buffer, string);
    }

    return buffer;
}

void checkBounds(uint8_t* boundStart, unsigned int boundLength, uint8_t* checkAddress, unsigned int checkNumberBytes) {
    uint8_t* lBound = boundStart;
    uint8_t* rBound = boundStart + boundLength;

    uint8_t* cLBound = checkAddress;
    uint8_t* cRBound = checkAddress + checkNumberBytes;

    if (cLBound < lBound || cLBound > rBound || cRBound < lBound || cRBound > rBound)
        killProcess("Accessing Undefined Memory\n");
}

int _strcmp(const void* c1, const void* c2) {
    return strcmp((char*) c1, (char*) c2);
}

int __strcmp(const void* c1, const void* c2) {
    char** _c1 = (char**) c1;
    char** _c2 = (char**) c2;
    return strcmp(*_c1, *_c2);
}

int isAlphaNumeric(char* string) {
    for (unsigned int i = 0; i < strlen(string); i++) {
        char ch = string[i];
        if ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122) || (ch >= 48 && ch <= 57))
            continue;
        return 0;
    }
    return 1;
}

void freeUINT8_T(void* uint) {
    uint8_t** _uint = (uint8_t**) uint;
    free(*_uint);
}

void freeUINT32_T(void* uint) {
    uint32_t** _uint = (uint32_t**) uint;
    free(*_uint);
}

void freeString(void* str) {
    char** _str = (char**) str;
    free(*_str);
}

void freeStr(void* str) {
    char* _str = (char*) str;
    free(_str);
}