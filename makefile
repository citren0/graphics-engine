
CC=g++
CFLAGS= -Og -g -fopenmp -DUNIX -m64
#-Wextra -Werror -Wall
LIBS=-lm -lX11 -lXext -lOpenCL

INC_DIRS=. $(CUDA)/OpenCL/common/inc

FILES=main.cpp ./implementations/display.cpp ./implementations/transforms.cpp ./implementations/utils.cpp ./implementations/window.cpp ./implementations/gpu.cpp ./implementations/clbuild.cpp

all:
	$(CC) $(CFLAGS) $(FILES) $(INC_DIRS:%=-I%) $(LIBS)

.PHONY: clean

clean:
	rm a.out