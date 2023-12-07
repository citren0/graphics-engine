#pragma once
#include <cmath>
#include "utils.hpp"
#include "constants.hpp"
#include <vector>

// Coefficient for projection matrix
#define S 1 / tan(fov*0.5*PI / 180.0)

// Far and near clipping planes
#define far 10000.0
#define near 0.1

#define dist(x1,y1,x2,y2) sqrt(pow(x2-x1, 2) + pow(y2-y1, 2))

#define scaleX(x) (int)(((double)(x + 1) / 2) * (SCREENWIDTH))
#define scaleY(y) (int)(((double)(y + 1) / 2) * (SCREENHEIGHT))

#define xInBounds(x) ((x < 0.95) && (x > -0.95))
#define yInBounds(y) ((y < 0.95) && (y > -0.95))

double * getProjMat();

void initBuf(char buf[SCREENHEIGHT * SCREENWIDTH]);

void displayVertices(std::vector<struct shape *> shapes, double * vertices, int * framebuf);