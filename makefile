
CC=g++
CFLAGS= -Og -g -Wall -Werror -Wextra -fopenmp

LIBS=-lm -lX11 -lXext

FILES=main.cpp ./implementations/display.cpp ./implementations/transforms.cpp ./implementations/utils.cpp ./implementations/window.cpp

all:
	$(CC) $(CFLAGS) $(FILES) $(LIBS)

.PHONY: clean

clean:
	rm a.out