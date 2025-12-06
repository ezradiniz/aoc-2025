CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 -lm

SRC = main.c
OUT = day

FORMAT_FILES = *.c

.PHONY: all run clean format

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT) input.txt

format:
	@clang-format -i --style=microsoft $(FORMAT_FILES)

clean:
	@rm -f $(OUT)
