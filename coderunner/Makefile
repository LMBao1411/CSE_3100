CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = $(wildcard *.c)

BINS = $(SRCS:.c=)

all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(BINS)