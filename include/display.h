#pragma once
#include "utils.h"

#define SCREENWIDTH 130
#define SCREENHEIGHT 45
#define MAXCONNECTIONS 4

#define dist(x1,y1,x2,y2) sqrtf(powf(x2-x1, 2) + powf(y2-y1, 2))


void initBuf(char buf[SCREENHEIGHT][SCREENWIDTH]);

void displayVertices(struct shape * shapes[], int numShapes);