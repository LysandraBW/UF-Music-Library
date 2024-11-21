#ifndef UTILS
#define UTILS

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

/*
This file contains simple functions that
don't necessarily have a specific file, and
are used throughout the files.
*/

/*
Prints an error message,
and kills the process.
@param  str     The error message.
*/
void killProcess(char* str);

/*
Concatenates the passed in
strings. The string must be null-terminated.
@param  n       The number of strings to concatenate.
@param  ...     The n strings to concatenate, these can be passed in literally.
*/
char* concatenateStrings(unsigned int n, ...);

/*
Kills the process if you're
reading outside the allocated
bounds.
@param  boundStart          The start of the allocated (safe) boundary.
@param  boundLength         The length of the allocated (safe) boundary, in bytes.
@param  checkAddress        The address to be accessed, this will be checked.
@param  checkNumberBytes    The number of bytes to be accessed, this will be checked.
*/
void checkBounds(uint8_t* boundStart, unsigned int boundLength, uint8_t* checkedAddress, unsigned int checkedNumberBytes);

/*
Encapsulates the strcmp function,
so as to not trigger any warnings.
Used for sorting and char*.
This is not being used right now.
@param  c1  The address of a string, casted to void*.
@param  c2  The address of a string, casted to void*.
@return     The strcmp function, with void* parameters instead of char* parameters.
*/
int _strcmp(const void* c1, const void* c2);

/*
Encapsulates the strcmp function,
so as to not trigger any warnings.
Used for sorting and char**.
@param  c1  The address of a string, casted to void*.
@param  c2  The address of a string, casted to void*.
@return     The strcmp function, with void* parameters instead of char* parameters.
*/
int __strcmp(const void* c1, const void* c2);

/*
Returns 0 if the given string
is alphanumeric (only made up
of letters and digits).
@param  s   The string to test.
@return     An integer indicating whether the string is alphanumeric or not.
*/
int isAlphaNumeric(char* string);

void freeUINT8_T(void* uint);
void freeUINT32_T(void* uint);
void freeString(void* str);
void freeStr(void* str);

#endif