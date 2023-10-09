#pragma once
#define PI 3.141592

#define MAX_VERTICES_PER_SHAPE 4
#define NUMBER_OF_HOMOGENEOUS_COORDS 4
#define MAX_CONNECTIONS_PER_VERTEX 4


struct location
{
    float x;
    float y;
    float z;
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
    float vectors[MAX_VERTICES_PER_SHAPE][NUMBER_OF_HOMOGENEOUS_COORDS];
    // The connection matrix will store relationships between vertices
    float connectivity[MAX_VERTICES_PER_SHAPE][MAX_VERTICES_PER_SHAPE];
    // Number of currently occupied vertex spots for the shape.
    int numVertices;
};

int addConnectionToShape(struct shape * target, int source, int destination);

int addVertexToShape(struct shape * target, struct location point);

void copyMatrix(float source[][4], float dest[][4], int n);

void matMatMult(float operation[4][4], float target[][4], float destination[][4], int n);

void printMat(float mat[][4], int n);

void initShape(struct shape * target);