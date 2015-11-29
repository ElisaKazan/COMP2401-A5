Assignment5: main.o util.o game.o network.o
	gcc -o $@ $?

main.o: main.c defs.h
	gcc -c main.c
util.o: util.c defs.h
	gcc -c util.c
game.o: game.c defs.h
	gcc -c game.c
network.o: network.c defs.h
	gcc -c network.c
