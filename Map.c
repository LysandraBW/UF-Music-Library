#include "Map.h"

Map* createMap(unsigned int valueSize) {
    Map* map = malloc(sizeof(Map));
    if (map == NULL)
        killProcess(MALLOC_ERROR);

    map->size = 0;
    map->valueSize = valueSize;

    map->capacity = 10;
    map->entries = malloc(map->capacity * sizeof(Entry));
    if (map->entries == NULL)
        killProcess(MALLOC_ERROR);
    
    return map;
}

void deleteMap(Map** map, void (*deleteEntry)(void*)) {
    if (deleteEntry != NULL) {
        for (unsigned int i = 0; i < (*map)->size; i++) {
            uint8_t* elementAddress = (uint8_t*) (*map)->entries + (i * sizeof(Entry));
            Entry* entry = (Entry*) elementAddress;
            deleteEntry((void*) entry->value);
        }
    }

    free((*map)->entries);
    (*map)->entries = NULL;
    (*map)->size = 0;
    (*map)->valueSize = 0;
    (*map)->capacity = 0;
    free(*map);
}

void* getEntry(Map** map, char* key) {
    for (unsigned int i = 0; i < (*map)->size; i++) {
        Entry* entry = (Entry*) ((uint8_t*) (*map)->entries + (i * sizeof(Entry)));
        if (strcmp(entry->key, key) == 0)
            return entry->value;
    }
    return NULL;
}

void putEntry(Map** map, char* key, void* value) {
    // Looking for any entries with the same key.
    // If we find any, we'll just replace the value.
    for (unsigned int i = 0; i < (*map)->size; i++) {
        Entry* entry = (Entry*) ((uint8_t*) (*map)->entries + (i * sizeof(Entry)));
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
    }

    // If the array is going to be full,
    // we need to resize it (double the capacity).
    if ((*map)->size + 1 == (*map)->capacity) {
        unsigned int doubledCapacity = 2 * (*map)->capacity;
        
        (*map)->entries = realloc((*map)->entries, doubledCapacity * sizeof(Entry));
        if ((*map)->entries == NULL)
            killProcess("Failed to Reallocate Memory\n");
        
        (*map)->capacity = doubledCapacity;
    }
    
    // Adding a new entry as it hasn't been found in the map.
    Entry entry = {key, value};
    uint8_t* entryAddress = (uint8_t*) (*map)->entries + ((*map)->size * sizeof(Entry));
    memcpy(entryAddress, &entry, sizeof(Entry));
    (*map)->size++;
}