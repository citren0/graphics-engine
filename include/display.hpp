#pragma once
#include "utils.hpp"
#include <vector>

#define SCREENWIDTH 2000
#define SCREENHEIGHT 1200
#define MAXCONNECTIONS 4

// Field of View in degrees
#define fov 45.0

// Coefficient for projection matrix
#define S 1/tan(fov*0.5*PI/180.0)

// Far and near clipping planes
#define far 1.0
#define near 0.1

#define dist(x1,y1,x2,y2) sqrt(pow(x2-x1, 2) + pow(y2-y1, 2))

#define scaleX(x) (int)((x * (SCREENWIDTH / 2)) + (SCREENWIDTH / 2))
#define scaleY(y) (int)((y * (SCREENHEIGHT / 2)) + (SCREENHEIGHT / 2))

#define xInBounds(x) ((x < 0.95) && (x > -0.95))
#define yInBounds(y) ((y < 0.95) && (y > -0.95))


void initBuf(char buf[SCREENHEIGHT][SCREENWIDTH]);

void displayVertices(std::vector<struct shape *> shapes, int * framebuf);