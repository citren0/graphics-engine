#pragma once
#include "utils.hpp"
#include <vector>

#define SCREENWIDTH 1600
#define SCREENHEIGHT 1200
#define MAXCONNECTIONS 4

#define dist(x1,y1,x2,y2) sqrtf(powf(x2-x1, 2) + powf(y2-y1, 2))

#define scaleX(x) (int)((x * (SCREENWIDTH / 2)) + (SCREENWIDTH / 2))
#define scaleY(y) (int)((y * (SCREENHEIGHT / 2)) + (SCREENHEIGHT / 2))


void initBuf(char buf[SCREENHEIGHT][SCREENWIDTH]);

void displayVertices(std::vector<struct shape *> shapes, int * framebuf);