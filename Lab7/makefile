CC = gcc
CFLAGS = -Wall -Wextra -g -lm
TARGET = bytedump
SRC = bytedump.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
	rm -f debug debug2

clean_build: clean all