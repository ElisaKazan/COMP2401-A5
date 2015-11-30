CFLAGS=-Wall -std=c99 -g

Assignment5: main.o util.o game.o network.o
	gcc -o $@ main.o util.o game.o network.o

main.o: main.c defs.h
	gcc ${CFLAGS} -c main.c
util.o: util.c defs.h
	gcc ${CFLAGS} -c util.c
game.o: game.c defs.h
	gcc ${CFLAGS} -c game.c
network.o: network.c defs.h
	gcc ${CFLAGS} -c network.c
