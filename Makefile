# makefile for pong
#
# make pong

CC=gcc
CFLAGS=-lncurses

clean:
	rm -rf pong

pong: pong.c pong.h
	$(CC) $(CFLAGS) pong.c pong.h -o pong
