Assignment5: main.o util.o
	gcc -o $@ $?

main.o:
	gcc -c main.c
util.o:
	gcc -c util.c
