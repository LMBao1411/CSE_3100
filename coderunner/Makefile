CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = main

SRCS = main.c parser.c executor.c output.c

OBJS = $(SRCS:.c=.o)

EXECUTABLES = $(SRCS:.c=)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLES) *.o

.PHONY: all clean