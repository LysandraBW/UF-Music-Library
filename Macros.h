#ifndef MACROS
#define MACROS

#include <stdint.h>

/* 
This file defines the sizes of commonly used data types.
Now, you don't have to repeatedly call sizeof and your
code looks cleaner. It now also includes other constants.
*/

#define CHAR_SIZE           sizeof(char)
#define CHAR_PTR_SIZE       sizeof(char*)
#define UINT32_T_SIZE       sizeof(uint32_t)
#define UINT8_T_PTR_SIZE    sizeof(uint8_t*)
#define UINT32_T_PTR_SIZE   sizeof(uint32_t*)
#define USERNAME_LENGTH     200
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_RESET    "\x1b[0m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define HEADER_LENGTH       2 * sizeof(uint32_t)
#define MALLOC_ERROR        "Failed to Allocate Memory\n"
#define INDEXING_ERROR      "Missing Element in Array or Map\n"
#define HEADER_ERROR        "Header Incorrectly Received\n"
#endif