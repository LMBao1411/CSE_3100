CC=gcc
CFLAGS=-Wall
TARGET=calculator

all: $(TARGET)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm -f $(TARGET)

guide:
	@echo "Usage:"
	@echo "./$(TARGET) <operation> "
	@echo "Operations:"
	@echo "  add - Perform addition "
	@echo "  mul - Perform multiplication "

