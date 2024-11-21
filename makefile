#################################################################
##
## FILE:	Makefile
## PROJECT:	CNT 4007 Project 2 - Professor Traynor
## DESCRIPTION: Compile Project 2
##
#################################################################

CC=gcc
CFLAGS = -I./uthash/src -Wall -g

OS := $(shell uname -s)

# Extra LDFLAGS if Solaris
ifeq ($(OS), SunOS)
	LDFLAGS=-lsocket -lnsl
    endif

all: 
	if [ ! -d "uthash" ]; then git clone https://github.com/troydhanson/uthash.git; fi
	$(CC) -o client client.c Array.c ClientHandler.c File.c Host.c Map.c ServerHandler.c Song.c Utils.c
	$(CC) -o server server.c Array.c ClientHandler.c File.c Host.c Map.c ServerHandler.c Song.c Utils.c
	$(CC) $(CFLAGS) -o create_songs create_songs.c