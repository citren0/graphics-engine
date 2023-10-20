
CC=g++
CFLAGS= -g -O0 -lX11

LIBS=-lm

FILES=main.cpp ./implementations/display.cpp ./implementations/transforms.cpp ./implementations/utils.cpp

all:
	$(CC) $(CFLAGS) $(FILES) $(LIBS)

.PHONY: clean

clean:
	rm a.out