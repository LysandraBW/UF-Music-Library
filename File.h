#ifndef FILES
#define FILES

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "Array.h"
#include "Macros.h"
#include "Utils.h"

/*
This file contains functions for files.
*/

/*
Stores the characters in a text file in a byte array.
    @param  filepath    The path to the file.
    @param  bytes       The byte array to store the characters in.
    @return             The number of bytes (or the length) of the byte array.
*/
uint32_t fileToBytes(char* filepath, uint8_t** bytes);

/*
Encodes the files in a byte array. The number of files, the
size of each file, and the characters of each file will all
be stored in the byte array.
    @param  filepaths   The paths of the files.
    @param  bytes       The byte array to store the information in.
    @return             The number of bytes (or the length) of the byte array.
*/
uint32_t fileListToBytes(Array* filepaths, uint8_t** bytes);

/*
Creates a file and writes all the characters, contained in
the byte array, to said file. This is used by the client.
    @param  bytes       The characters of the file.
    @param  numberBytes The number of bytes (characters) in the file.
*/
void downloadFile(uint8_t* bytes, uint32_t numberBytes, char* username);

/*
Creates a file and writes all the characters for all the files
encoded in the byte array. This uses the prior function.
    @param  bytes       The bytes of the encoded files.
    @param  numberBytes The number of bytes in the byte array.
*/
void downloadFileList(uint8_t* bytes, uint32_t numberBytes, char* username);

/*
This function returns the paths of the files associated
with the given client ID and ISWCs. This is used by the
server to locate the particular files of a client.
    @param  clientID    The client ID used in the filepath.
    @param  ISWCs       The ISWCs used in the filepaths.
    @return             An array of the created filepaths.
*/
Array* clientFileList(char* clientID, Array* ISWCs);

#endif