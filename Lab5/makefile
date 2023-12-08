CC = gcc
CFLAGS = -Wall -Wextra -g -lm
TARGET = Lab5
SRC = scheduling.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET)

clean_build: clean all