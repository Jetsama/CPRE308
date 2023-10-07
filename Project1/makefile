CC = gcc
CFLAGS = -Wall -Wextra -g -lm
TARGET = Project1
SRC = main.c pathcommands.c proccesses.c createlog.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
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
	rm -f $(TARGET) $(OBJ)

clean_build: clean all