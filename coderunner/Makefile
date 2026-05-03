CC = gcc
CFLAGS = -Wall -Wextra -g

# Main executable name
TARGET = main

# All source files
SRCS = main.c parser.c executor.c output.c

# Object files
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

# Link all object files into final program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile each .c into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean