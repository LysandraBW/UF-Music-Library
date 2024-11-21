#ifndef MAP
#define MAP

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Utils.h"
#include "Macros.h"

/*
Likewise, this file is used to imitate a map in C++.
However, we only have the bare functionality needed.
*/

typedef struct {
    char* key;              /* The key of the entry. */
    void* value;            /* A pointer to the value of the entry, must be casted. */
} Entry;

typedef struct {
    Entry* entries;         /* The array of map entries. */
    unsigned int size;      /* The number of entries in the map. */
    unsigned int valueSize; /* The size (in bytes) of each entry's value */
    unsigned int capacity;  /* The number of entries the map could hold. */
} Map;

/*
This function initializes a Map object.
The valueSize is used for allocation.
    @param  valueSize   The size of an entry's value.
*/
Map* createMap(unsigned int valueSize);

/*
Deallocates a Map object.
    @param  map     The Map object to deallocate.
*/
void deleteMap(Map** map, void (*deleteEntry)(void*));

/*
Retrieves an entry by its key. Returns
the NULL pointer if no matching entry is found.
    @param  map     The Map object to "index".
    @param  key     The key to index into the Map object.
    @return         A pointer to the value associated with the key.
*/
void* getEntry(Map** map, char* key);

/*
Inserts a key and value pair (entry) into a
map. Replaces any entry with the same key.
    @param  map     The Map object to update.
    @param  key     The key of the entry.
    @param  value   The value of the entry.
*/
void putEntry(Map** map, char* key, void* value);

#endif