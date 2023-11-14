CC = gcc
CFLAGS = -Wall -Wextra -g -lm
TARGET = appserver
SRC = main.c Bank.c thread.c request.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#make tester
Project2Test: Project2Test_v2-1.o
	$(CC) $(CFLAGS) -o $@ $^

Project2Test_v2-1.o: Project2Test_v2-1.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) appserver
	rm -f debug debug2

clean_build: clean all