
CC=gcc
CFLAGS= -g -O0

LIBS=-lm

FILES=main.c ./implementations/display.c ./implementations/transforms.c ./implementations/utils.c

all:
	$(CC) $(CFLAGS) $(FILES) $(LIBS)

.PHONY: clean

clean:
	rm a.out