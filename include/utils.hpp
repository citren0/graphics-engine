#pragma once
#include <vector>
#include "constants.hpp"

#define UNUSED(x) (void)x


struct location
{
    double x;
    double y;
    double z;
};

struct axis
{
    char x;
    char y;
    char z;
};


struct shape
{
    // Stores the homogeneous coordinates of each vertex.
    double * vectors;
    // Number of currently occupied vertex spots for the shape.
    int numVertices;
};


double * initVertices();

int addVertexToShape(struct shape * target, struct location point);

void copyMatrix1D(double * source, double * dest, int n);

void printMat(double * mat, int numRows);

void initShape(struct shape * target, double * vectors, int numShapes);