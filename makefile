
CC=g++
CFLAGS= -g -Og

LIBS=-lm -lX11 -lXext -lcurses

FILES=main.cpp ./implementations/display.cpp ./implementations/transforms.cpp ./implementations/utils.cpp

all:
	$(CC) $(CFLAGS) $(FILES) $(LIBS)

.PHONY: clean

clean:
	rm a.out