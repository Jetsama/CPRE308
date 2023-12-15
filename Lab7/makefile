CC = gcc
CFLAGS = -Wall -Wextra -g -lm
TARGET = bytedump
SRC = bytedump.c 
OBJ = $(SRC:.c=.o)

bsdump: bsdump-template.o
	$(CC) $(CFLAGS) -o $@ $^

fat12ls: fat12ls-template.o
	$(CC) $(CFLAGS) -o $@ $^

all: $(TARGET) bsdump fat12ls

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
	rm -f bsdump fat12ls

clean_build: clean all