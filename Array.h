#ifndef ARRAY
#define ARRAY

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
This file is used to imitate a vector in C++.
It doesn't have all its functionalities, it
only has what's needed for this project.
*/

typedef struct {
    void* array;                /* A pointer to the data. */
    unsigned int arrayLength;   /* The number of elements in the array. */
    unsigned int elementSize;   /* The size in bytes of each element in the array. */
    unsigned int capacity;      /* The number of elements the array could hold. */
} Array;

/*
Returns an initialized Array object.
    @param  elementSize The size in bytes of each element in the array, used for indexing and resizing.
    @return             The initialized Array object.
*/
Array* createArray(unsigned int elementSize);

/*
Deallocates an Array object.
    @param  array           The Array object to deallocate.
    @param  deleteElement   The function used to deallocate the element.
*/
void deleteArray(Array** array, void (*deleteElement)(void*));

/*
Retrieves the element at the given index.
You'll have to cast the element to its actual data type.
    @param  array   The Array object to index.
    @param  index   The index of the element.
    @return         The address of the element.
*/
void* getElement(Array** array, unsigned int index);

/*
Adds an element to the end of an Array obejct.
    @param  array   The Array object to manipulate.
    @param  value   The address of the element to add.
*/
void addElement(Array** array, void* value);

#endif