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

void copyMatrix(double source[][NUMBER_OF_HOMOGENEOUS_COORDS], double dest[][NUMBER_OF_HOMOGENEOUS_COORDS], int n);

void copyMatrix1D(double * source, double * dest, int n);

void matMatMult(double operation[NUMBER_OF_HOMOGENEOUS_COORDS][NUMBER_OF_HOMOGENEOUS_COORDS], double target[][NUMBER_OF_HOMOGENEOUS_COORDS], double destination[][NUMBER_OF_HOMOGENEOUS_COORDS], int n);

void matMatMult1D(double operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS], double * target, double * destination, int n);

void printMat(double * mat, int numRows);

void initShape(struct shape * target, double * vectors, int numShapes);

void combineVertices(std::vector<struct shape *> targets, double * combinedVertices);

void seperateVertices(std::vector<struct shape *> targets, double * final);

int countVertices(std::vector<struct shape *> targets);