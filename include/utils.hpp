#pragma once
#define PI 3.141592

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
    double vectors[MAX_VERTICES_PER_SHAPE][NUMBER_OF_HOMOGENEOUS_COORDS];
    // The connection matrix will store relationships between vertices
    double connectivity[MAX_VERTICES_PER_SHAPE][MAX_VERTICES_PER_SHAPE];
    // Number of currently occupied vertex spots for the shape.
    int numVertices;
};

int addConnectionToShape(struct shape * target, int source, int destination);

int addVertexToShape(struct shape * target, struct location point);

void copyMatrix(double source[][4], double dest[][4], int n);

void matMatMult(double operation[4][4], double target[][4], double destination[][4], int n);

void printMat(double mat[][4], int n);

void initShape(struct shape * target);