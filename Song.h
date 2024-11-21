#ifndef SONG
#define SONG

#include <stdio.h>
#include <stdint.h>
#include <dirent.h>
#include <stdlib.h>
#include "Array.h"
#include "Utils.h"
#include "Macros.h"
#include "Map.h"
#include "uthash/src/uthash.h"

/*
The Song wraps the data 
that is stored in a file.
*/
typedef struct {
    char* ISWC;         /* This is the song's unique identifier, also used in filenames and for differentiating.*/
    char* title;        /* This is song's title, like "Hold Me Now - Thompson Twins". */
    UT_hash_handle hh;  /* Used for hashing */
} Song;

/*
The Library is an Array
of Song objects.
*/
typedef struct {
    Array* songs;
} Library;

/*
Stores the file representation
of a song into a Song object.
    @param  filepath    The path of the file.
    @return             A Song object that contains all the data stored in the said file.
*/
Song* loadSong(char* filepath);

/*
Takes all the files associated
with the given prefix, loading 
them each as a Song object, and 
storing them all in an Array.
    @param  prefix  The prefix all selected files must have; this is used to find the files that belong to a client.
    @return         A Library object that contains all the information stored in the various files.
*/
Library* loadLibrary(char* prefix);

/*
Encodes a Song into a byte array.
    @param  song    The Song object to encode.
    @param  bytes   The byte array said object will be encoded into.
    @return         The size of the encoded byte array.
*/
uint32_t songToBytes(Song* song, uint8_t** bytes);

/*
Decodes a Song from a byte array.
    @param  bytes       The byte array to decode the Song from.
    @param  numberBytes The size of the byte array.
    @return             The decoded Song object.
*/
Song* bytesToSong(uint8_t* bytes, uint32_t numberBytes);

/*
Encodes a Library into a byte array.
    @param  library The Libray object to encode.
    @param  bytes   The byte array said object will be encoded into.
    @return         The size of the encoded byte array.
*/
uint32_t libraryToBytes(Library* library, uint8_t** bytes);

/*
Decodes a Library from a byte array.
    @param  bytes       The byte array to decode the Library from.
    @param  numberBytes The size of the byte array.
    @return             The decoded Library object.
*/
Library* bytesToLibrary(uint8_t* bytes, uint32_t numberBytes);

/*
Compares two libraries (the server's library and the client's library).
This function will store the ISWCs of the songs that the server has and
the client does not have in an Array. This is used by the server to in
the PULL action. It will also return a character output that can be 
outputted to the screen (for the client).
    @param  serverLibrary   The songs the server has stored for the client.
    @param  clientLibrary   The songs the client has stored.
    @param  diff            An Array that stores all the ISWCs that the server has and the client does not.
    @return                 Returns a string that can be printed to the screen, used by the client.
*/
char* compareLibraries(Library* serverLibrary, Library* clientLibrary, Array** diff);

void freeSong(void* song);

#endif