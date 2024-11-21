#include "Array.h"
#include "Utils.h"

Array* createArray(unsigned int elementSize) {
    Array* array = malloc(sizeof(Array));
    if (array == NULL)
        killProcess("Failed to Allocate Memory for Array\n");

    array->capacity = 10;
    array->array = malloc(elementSize * array->capacity);
    if (array->array == NULL)
        killProcess("Failed to Allocate Memory for Elements\n");
    
    array->arrayLength = 0;
    array->elementSize = elementSize;
    return array;
}

void deleteArray(Array** array, void (*deleteElement)(void*)) {
    // You can pass in a function to delete the individual
    // elements in case they also contain allocated memory.
    if (deleteElement != NULL) {
        for (unsigned int i = 0; i < (*array)->arrayLength; i++) {
            uint8_t* elementAddress = (uint8_t*) (*array)->array + (i * (*array)->elementSize);
            deleteElement((void*) elementAddress);
        }
    }

    free((*array)->array);
    (*array)->array = NULL;
    (*array)->arrayLength = 0;
    (*array)->elementSize = 0;
    (*array)->capacity = 0;
    free(*array);
}

void* getElement(Array** array, unsigned int index) {
    uint8_t* elementAddress = (uint8_t*) (*array)->array + (index * (*array)->elementSize);
    return (void*) elementAddress;
}

void addElement(Array** array, void* value) {
    // If the array is going to be full,
    // we need to double the capacity.
    if ((*array)->arrayLength + 1 == (*array)->capacity) {
        unsigned int doubledCapacity = 2 * (*array)->capacity;
        
        (*array)->array = realloc((*array)->array, doubledCapacity * (*array)->elementSize);
        if ((*array)->array == NULL)
            killProcess("Failed to Reallocate Memory\n");
        
        (*array)->capacity = doubledCapacity;
    }

    // Add the element to the end of the array.
    uint8_t* elementAddress = (uint8_t*) (*array)->array + ((*array)->arrayLength * (*array)->elementSize);
    memcpy(elementAddress, value, (*array)->elementSize);
    (*array)->arrayLength++;
}