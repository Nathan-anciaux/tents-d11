CC = gcc
CFLAGS = -std=c99 -Wall
LDFLAGS = -L.
LDLIBS = -lgame

ALL: game_text

game_text: game_text.o libgame.a

game_text.o : game_text.c game_aux.h game.h 



.PHONY : clean
clean : 
	rm -f *.o game_text