CC = gcc
CFLAGS = -Wall -Wextra -g -lm
SRC = mainIntrotoSignalslab4.c shm_client.c shm_server.c signalhandlers.c dividebyzerocatch.c signalsusingalarm.c signalandfork.c pipes.c
OBJ = $(SRC:.c=.o)

all: lab4-3-1 lab4-3-2 lab4-3-3 lab4-3-4 lab4-3-5 lab4-3-6 lab4-3-7-server lab4-3-7-client

lab4-3-1: IntrotoSignalslab4.o
	$(CC) $(CFLAGS) -o $@ $^
lab4-3-2: signalhandlers.o
	$(CC) $(CFLAGS) -o $@ $^
lab4-3-3: dividebyzerocatch.o
	$(CC) $(CFLAGS) -o $@ $^
lab4-3-4: signalsusingalarm.o
	$(CC) $(CFLAGS) -o $@ $^
lab4-3-5: signalandfork.o
	$(CC) $(CFLAGS) -o $@ $^
lab4-3-6: pipes.o
	$(CC) $(CFLAGS) -o $@ $^
lab4-3-7-client: shm_client.o
	$(CC) $(CFLAGS) -o $@ $^
lab4-3-7-server: shm_server.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@




#Project1: main.o pathcommands.o 
#	gcc -o Project1 main.o pathcommands.o 
#proccesses: proccesses.o

#main.o: main.c commands.h
#	gcc -c -lpthread main.c

#pathcommands.o: pathcommands.c commands.h
#	gcc -c -lpthread pathcommands.c

#proccesses.o: proccesses.c commands.h
#	gcc -c -lpthread proccesses.c


clean:
	rm -f $(OBJ) lab4-3-1

clean_build: clean all