

all: main path proccesses

main: main.o path proccesses
path: path.o
proccesses: proccesses.o

main.o: main.c commands/commands.h
	gcc -c -lpthread main.c

path.o: path.c
	gcc -c -lpthread path.c

proccesses.o: proccesses.c
	gcc -c -lpthread proccesses.c