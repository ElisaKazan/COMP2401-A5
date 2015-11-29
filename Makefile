Assignment5: main.o util.o game.o
	gcc -o $@ $?

main.o:
	gcc -c main.c
util.o:
	gcc -c util.c
game.o:
	gcc -c game.c
