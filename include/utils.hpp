#pragma once
#include <vector>

#define UNUSED(x) (void)x

#define PI 3.14159265

#define MAX_SHAPES 1000000
#define MAX_VERTICES_PER_SHAPE 4
#define NUMBER_OF_HOMOGENEOUS_COORDS 4
#define MAX_CONNECTIONS_PER_VERTEX 4


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
    // This array will store the number of connections for each shape.
    int connsEachVertex[MAX_VERTICES_PER_SHAPE];
    // The connection matrix will store relationships between vertices
    int connections[MAX_VERTICES_PER_SHAPE][MAX_CONNECTIONS_PER_VERTEX];
    // Number of currently occupied vertex spots for the shape.
    int numVertices;
};


double * initVertices();

int addConnectionToShape(struct shape * target, int source, int destination);

int addVertexToShape(struct shape * target, struct location point);

void copyMatrix(double source[][NUMBER_OF_HOMOGENEOUS_COORDS], double dest[][NUMBER_OF_HOMOGENEOUS_COORDS], int n);

void copyMatrix1D(double * source, double * dest, int n);

void matMatMult(double operation[NUMBER_OF_HOMOGENEOUS_COORDS][NUMBER_OF_HOMOGENEOUS_COORDS], double target[][NUMBER_OF_HOMOGENEOUS_COORDS], double destination[][NUMBER_OF_HOMOGENEOUS_COORDS], int n);

void matMatMult1D(double operation[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS], double * target, double * destination, int n);

void printMat(double mat[][4], int n);

void initShape(struct shape * target, double * vectors, int numShapes);

void combineVertices(std::vector<struct shape *> targets, double * combinedVertices);

void seperateVertices(std::vector<struct shape *> targets, double * final);

int countVertices(std::vector<struct shape *> targets);

void applyTransform(double * vertices, double transform[NUMBER_OF_HOMOGENEOUS_COORDS * NUMBER_OF_HOMOGENEOUS_COORDS]);