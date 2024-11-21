#ifndef HOST
#define HOST

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "Macros.h"
#include "Utils.h"

/*
This file contains common functions used for networking.
*/

/*
Headers, a fixed-length structure, notify the receiver of:
    (1) the action it's asking for or performing; and
    (2) the number of bytes it's going to send in the next message.
With headers, the receiving host will know how much bytes to expect.
So, headers are kind of analogous to handshaking.
*/
typedef struct {
    uint32_t action;            /* The action that is to be performed or is being performed. */
    uint32_t totalBytesToRecv;  /* This tells the receiver how much bytes to expect in the next message. */  
} Header;

/*
Encodes a Header in a byte array.
@param  serverAction        The action of the request/response (i.e. LIST, DIFF, PULL).
@param  totalBytesToRecv    The number of bytes that will be sent in the next message.
@param  bytes               This stores the encoded header.
@return                     The size of the byte array -- should be 8.
*/
uint32_t headerToBytes(uint32_t action, uint32_t totalBytesToRecv, uint8_t** bytes);

/*
Decodes a Header from a byte array.
@param  bytes           The encoded bytes of a Header.
@param  numberBytes     The size of the byte array.
@return                 The Header that was decoded from the bytes.
*/
Header bytesToHeader(uint8_t* bytes, uint32_t numberBytes);

/*
Encodes a string into a byte array.
Not much has to be done here to be honest, just
making sure that the string is null-terminated.
@param  str     The string to be encoded.
@param  bytes   The bytes of the encoded string.
@return         The size of the byte array.
*/
uint32_t stringToBytes(char* str, uint8_t** bytes);

/*
Decodes a string from a byte array. Not much has
to be here either as a string is a byte array.
@param  bytes       The bytes of the encoded string.
@param  numberBytes The size of the byte array.
@return             The string.
*/
char* bytesToString(uint8_t* bytes, uint32_t numberBytes);

/*
Sends bytes to a host.
@param  clientSocket    Determines the receiver of the bytes.
@param  bytes           The bytes to the send.
@param  numberBytes     The size of the byte array (bytes).
*/
void sendBytes(int clientSocket, uint8_t* bytes, uint32_t numberBytes);

/*
Receives bytes from a host.
@param  clientSocket    Determines where to receive the bytes from.
@param  bytes           This byte array stores the received bytes.
@param  numberBytes     The number of bytes to receive.
@return                 Returns the number of received bytes.
*/
unsigned int recvBytes(int clientSocket, uint8_t** bytes, uint32_t numberBytes);

#endif