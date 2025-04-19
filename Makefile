CC = gcc
CFLAGS = -Wall -Iinclude

SRC = src/main.c src/executor.c src/builtins.c
OBJ = $(SRC:.c=.o)
TARGET = myshell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) src/*.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline